#include "idgen_app.hpp"
#include "version.hpp"
#include <be/core/version.hpp>
#include <be/core/byte_order.hpp>
#include <be/core/log_exception.hpp>
#include <be/util/fnv.hpp>
#include <be/util/base64_encode.hpp>
#include <be/util/util_fs_autolink.hpp>
#include <be/util/util_prng_autolink.hpp>
#include <be/cli/cli.hpp>
#include <iostream>
#include <sstream>

namespace be::idgen {
namespace {

///////////////////////////////////////////////////////////////////////////////
S escape_tab_string(const S& input) {
   S out;
   out.reserve(input.size());
   for (char c : input) {
      switch (c) {
         case '\t':  out.append("\\t"); break;
         case '\r':  out.append("\\r"); break;
         case '\n':  out.append("\\n"); break;
         case '\\':  out.append("\\\\"); break;
         default:    out.append(1, c);
      }
   }
   return out;
}

///////////////////////////////////////////////////////////////////////////////
S escape_json_string(const S& input) {
   S out;
   out.reserve(input.size() + 2);
   out.append(1, '"');
   for (char c : input) {
      if (c > '"' && c < '\\' ||
          c > '\\' && c <= '~') {
         out.append(1, c);
      } else switch (c) {
         case '!':
         case ' ':
            out.append(1, c);
            break;

         case '"':
         case '\\':
            out.append(1, '\\');
            out.append(1, c);
            break;

         case '\t':  out.append("\\t"); break;
         case '\r':  out.append("\\r"); break;
         case '\n':  out.append("\\n"); break;
         case '\b':  out.append("\\b"); break;
         case '\f':  out.append("\\f"); break;
         
         default:
            out.append("\\u00");
            out.append(1, "0123456789ABCDEF"[(c >> 4) & 0xF]);
            out.append(1, "0123456789ABCDEF"[c & 0xF]);
            break;
      }
   }
   out.append(1, '"');
   return out;
}

///////////////////////////////////////////////////////////////////////////////
S escape_xml_attribute(const S& input) {
   S out;
   out.reserve(input.size() + 2);
   out.append(1, '"');
   for (char c : input) {
      if (c <= ' ') {
         out.append("&#");
         if (c >= 10) {
            out.append(1, "0123456789"[c / 10]);
         }
         out.append(1, "0123456789"[c % 10]);
         out.append(1, ';');
      } else if (c > '~') {
         out.append("&#");
         if (c >= 100) {
            out.append(1, "0123456789"[c / 100]);
            c %= 100;
         }
         if (c >= 10) {
            out.append(1, "0123456789"[c / 10]);
            c %= 10;
         }
         out.append(1, "0123456789"[c]);
         out.append(1, ';');
      } else switch (c) {
         case '"':  out.append("&quot;"); break;
         case '\'': out.append("&apos;"); break;
         case '<':  out.append("&lt;"); break;
         case '>':  out.append("&gt;"); break;
         case '&':  out.append("&amp;"); break;

         default:
            out.append(1, c);
            break;
      }
   }
   out.append(1, '"');
   return out;
}

///////////////////////////////////////////////////////////////////////////////
S escape_xml_pcdata(const S& input) {
   S out;
   out.reserve(input.size());
   for (char c : input) {
      if (c <= ' ') {
         out.append("&#");
         if (c >= 10) {
            out.append(1, "0123456789"[c / 10]);
            c %= 10;
         }
         out.append(1, "0123456789"[c]);
         out.append(1, ';');
      } else if (c > '~') {
         out.append("&#");
         if (c >= 100) {
            out.append(1, "0123456789"[c / 100]);
            c %= 100;
         }
         if (c >= 10) {
            out.append(1, "0123456789"[c / 10]);
            c %= 10;
         }
         out.append(1, "0123456789"[c]);
         out.append(1, ';');
      } else switch (c) {
         case '"':  out.append("&quot;"); break;
         case '\'': out.append("&apos;"); break;
         case '<':  out.append("&lt;"); break;
         case '>':  out.append("&gt;"); break;
         case '&':  out.append("&amp;"); break;

         default:
            out.append(1, c);
            break;
      }
   }
   return out;
}

} // be::idgen::()

///////////////////////////////////////////////////////////////////////////////
IdGenApp::IdGenApp(int argc, char** argv) {
   default_log().verbosity_mask(v::info_or_worse);
   try {
      using namespace cli;
      using namespace color;
      using namespace ct;
      Processor proc;

      bool show_help = false;
      bool verbose = false;
      bool show_version = false;
      S help_query;

      proc
         (prologue(Table() << header << "FNV-1a ID Generator").query())

         (synopsis(Cell() << fg_dark_gray << "[ " << fg_cyan << "OPTIONS"
            << fg_dark_gray << " ] [ " << fg_cyan << "INPUT"
            << fg_dark_gray << " [ " << fg_cyan << "INPUT"
            << fg_dark_gray << " ...]]"))

         (abstract("Hashes strings byte-by-byte using the FNV-1a hash and outputs the resultant value."))

         (numeric_param({ "s" }, { "size","length" }, "X", bits_, U16(1), U16(64))
            .desc("Specifies the hash size in bits.")
            .extra(Cell() << "Values other than " << fg_cyan << "32" << reset << " and " << fg_cyan << "64" << reset << " are generated by XOR-folding."))

         (enum_param({ "f" }, { "format" }, "X", format_)
            .desc("Specifies the format to use when outputting results."))

         (enum_param({ "t" }, { "type" }, "X", type_)
            .desc("Specifies the type of hash string to generate."))
         
         (param({ }, { "sep", "separator" }, "X", separator_)
            .desc("Specifies a string that should be inserted between each value that is output."))
         
         (numeric_param({ }, { "basis" }, "X", basis_, [this](U64 basis) {
               use_basis_ = true;
               return basis;
            }).default_value("0")
              .desc("Specifies a non-standard basis value (The value given to an empty string)"))

         (flag({ }, { "fnv-1a" }, mode_, Mode::fnv1a)
            .desc("Use the FNV-1a algorithm (default)."))

         (flag({ "1" }, { "fnv-1" }, mode_, Mode::fnv1)
            .desc("Use the FNV-1 algorithm."))

         (flag({ "0" }, { "fnv-0" }, mode_, Mode::fnv0)
            .desc("Use the FNV-0 algorithm.")
            .extra(Cell() << fg_yellow << "--basis" << reset << " is ignored while this mode is active."))

         (flag({  }, { "chongo" }, [this]() {
               auto old_mode = mode_;
               mode_ = Mode::fnv0;
               process_("chongo <Landon Curt Noll> /\\../\\");
               mode_ = old_mode;
            }).desc("Generate the standard basis."))

         (flag({ }, { "nil" }, [this]() {
               process_("");
            }).desc("Process the empty string."))

         (flag({ "i" }, { "interactive" }, interactive_)
            .desc("After processing any inputs provided on the command line, process each line entered from STDIN until EOF is reached."))

         (any([&](const S& str) {
               process_(str);
               return true;
            }))

         (end_of_options())

         (verbosity_param({ "v" }, { "verbosity" }, "LEVEL", default_log().verbosity_mask()))

         (flag({ "V" }, { "version" }, show_version).desc("Prints version information to standard output."))

         (param({ "?" }, { "help" }, "OPTION", [&](const S& value) {
               show_help = true;
               help_query = value;
            }).default_value(S())
               .allow_options_as_values(true)
               .desc(Cell() << "Outputs this help message.  For more verbose help, use " << fg_yellow << "--help")
               .extra(Cell() << nl << "If " << fg_cyan << "OPTION" << reset
                     << " is provided, the options list will be filtered to show only options that contain that string."))

         (flag ({ },{ "help" }, verbose).ignore_values(true))

         (exit_code(0, "There were no errors."))
         (exit_code(1, "An unknown error occurred."))
         (exit_code(2, "There was a problem parsing the command line arguments."))
         ;

      proc.process(argc, argv);

      if (interactive_) {
         while (std::cin) {
            S str;
            std::getline(std::cin, str);
            if (str.length() > 0) {
               process_(str);
            }
         }
      }

      if (!first_) {
         switch (format_) {
            case Format::json:
               std::cout << unindent << nl << "}";
               break;

            case Format::xml:
               std::cout << unindent << nl << "</ids>";
               break;
         }
      }

      if (!show_help && !show_version && first_) {
         show_help = true;
         show_version = true;
         status_ = 1;
      }

      if (show_version) {
         proc
            (prologue(BE_IDGEN_VERSION_STRING).query())
            (license(BE_LICENSE).query())
            (license(BE_COPYRIGHT).query())
            ;
      }

      if (show_help) {
         proc.describe(std::cout, verbose, help_query);
      } else if (show_version) {
         proc.describe(std::cout, verbose, ids::cli_describe_section_prologue);
         proc.describe(std::cout, verbose, ids::cli_describe_section_license);
      }

   } catch (const cli::OptionError& e) {
      status_ = 2;
      cli::log_exception(e, default_log());
   } catch (const cli::ArgumentError& e) {
      status_ = 2;
      cli::log_exception(e, default_log());
   } catch (const FatalTrace& e) {
      status_ = 2;
      log_exception(e, default_log());
   } catch (const RecoverableTrace& e) {
      status_ = 2;
      log_exception(e, default_log());
   } catch (const fs::filesystem_error& e) {
      status_ = 2;
      log_exception(e, default_log());
   } catch (const std::system_error& e) {
      status_ = 2;
      log_exception(e, default_log());
   } catch (const std::exception& e) {
      status_ = 2;
      log_exception(e, default_log());
   }
}

///////////////////////////////////////////////////////////////////////////////
int IdGenApp::operator()() {
   return status_;
}

///////////////////////////////////////////////////////////////////////////////
void IdGenApp::process_(const S& input) {
   if (first_) {
      first_ = false;

      switch (format_) {
         case Format::json:
            std::cout << "{" << indent << nl;
            break;

         case Format::xml:
            std::cout << "<?xml version=\"1.1\" charset=\"UTF-8\" standalone=\"yes\"?>" << nl
               << "<ids>" << indent << nl;
            break;
      }
   } else {
      switch (format_) {
         case Format::json:
            std::cout << ",";
            break;
      }

      for (char c : separator_) {
         if (c == '\n') {
            std::cout << nl;
         } else if (c != '\r') {
            std::cout << c;
         }
      }
   }

   U64 hash;

   if (bits_ <= 32) {
      U32 raw = 0;
      if (mode_ == Mode::fnv0) {
         util::detail::Fnv<U32> fnv;
         raw = fnv(input.begin(), input.end());
      } else {
         if (use_basis_) {
            if (mode_ == Mode::fnv1a) {
               util::detail::Fnva<U32> fnv;
               raw = fnv((U32)basis_, input.begin(), input.end());
            } else if (mode_ == Mode::fnv1) {
               util::detail::Fnv<U32> fnv;
               raw = fnv((U32)basis_, input.begin(), input.end());
            }
         } else {
            if (mode_ == Mode::fnv1a) {
               raw = util::fnv1a<U32>(input.begin(), input.end());
            } else if (mode_ == Mode::fnv1) {
               raw = util::fnv1<U32>(input.begin(), input.end());
            }
         }
      }

      if (bits_ < 32) {
         U32 bits = 32;
         while ((bits >> 1) >= bits_) {
            bits >>= 1;
            raw = raw ^ (raw >> bits);
            raw &= (1 << bits) - 1;
         }

         if (bits > bits_) {
            raw = raw ^ (raw >> bits_);
            raw &= (1 << bits_) - 1;
         }
      }

      hash = raw;
   } else {
      U64 raw = 0;
      if (mode_ == Mode::fnv0) {
         util::detail::Fnv<U64> fnv;
         raw = fnv(input.begin(), input.end());
      } else {
         if (use_basis_) {
            if (mode_ == Mode::fnv1a) {
               util::detail::Fnva<U64> fnv;
               raw = fnv(basis_, input.begin(), input.end());
            } else if (mode_ == Mode::fnv1) {
               util::detail::Fnv<U64> fnv;
               raw = fnv(basis_, input.begin(), input.end());
            }
         } else {
            if (mode_ == Mode::fnv1a) {
               raw = util::fnv1a<U64>(input.begin(), input.end());
            } else if (mode_ == Mode::fnv1) {
               raw = util::fnv1<U64>(input.begin(), input.end());
            }
         }
      }

      if (bits_ < 64) {
         U32 bits = 64;
         while ((bits >> 1) >= bits_) {
            bits >>= 1;
            raw = raw ^ (raw >> bits);
            raw &= ~U64(0) >> (64 - bits);
         }

         if (bits > bits_) {
            raw = raw ^ (raw >> bits_);
            raw &= ~U64(0) >> (64 - bits);
         }
      }

      hash = raw;
   }

   S serialized;
   serialized.reserve(input.size() + 50);

   switch (type_) {
      case Type::hash:
         serialized.append(1, '#');
         for (int d = (bits_ + 3) / 4 - 1; d >= 0; --d) {
            U64 digit = (hash >> (d * 4)) & 0xF;
            serialized.append(1, "0123456789ABCDEF"[digit]);
         }
         break;

      case Type::hex:
         serialized.append("0x");
         for (int d = (bits_ + 3) / 4 - 1; d >= 0; --d) {
            U64 digit = (hash >> (d * 4)) & 0xF;
            serialized.append(1, "0123456789ABCDEF"[digit]);
         }
         break;
         
      case Type::signed_dec: {
         std::ostringstream oss;
         oss << std::dec << static_cast<I64>(hash);
         serialized.assign(oss.str());
         break;
      }

      case Type::unsigned_dec: {
         std::ostringstream oss;
         oss << std::dec << hash;
         serialized.assign(oss.str());
         break;
      }

      case Type::base64: {
         std::size_t bytes = (bits_ + 7) / 8;
         hash = bo::to_big(hash);
         UC data[sizeof(hash)];
         memcpy(data, &hash, sizeof(hash));
         Buf<const UC> buf = tmp_buf(data);
         serialized.assign(util::base64_encode(sub_buf(buf, sizeof(hash) - bytes)));
         break;
      }

      default:
         serialized = "Unknown Type";
         break;
   }

   
   switch (format_) {
      case Format::canonical:
         std::cout << serialized;
         break;

      case Format::standard:
         std::cout << serialized;
         std::cout << ':';
         std::cout << input;
         break;

      case Format::tab:
         std::cout << escape_tab_string(input);
         std::cout << "\t";
         std::cout << escape_tab_string(serialized);
         break;

      case Format::json:
         std::cout << escape_json_string(input);
         std::cout << ": ";
         std::cout << escape_json_string(serialized);
         break;

      case Format::xml:
         std::cout << "<id";
         if (mode_ != Mode::fnv1a) {
            std::cout << " mode=";
            std::cout << escape_xml_attribute(mode_name(mode_));
         }
         if (use_basis_) {
            std::cout << " basis=";
            std::ostringstream oss;
            oss << std::hex << "0x" << basis_;
            std::cout << escape_xml_attribute(oss.str());
         }
         if (bits_ != 64) {
            std::cout << " size=";
            std::cout << escape_xml_attribute(std::to_string(bits_));
         }
         std::cout << " format=";
         std::cout << escape_xml_attribute(type_name(type_));
         std::cout << " name=";
         std::cout << escape_xml_attribute(input);
         std::cout << ">";
         std::cout << escape_xml_pcdata(serialized);
         std::cout << "</id>";
         break;
   }
}

} // be::idgen
