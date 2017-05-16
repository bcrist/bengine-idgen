#include "format.hpp"
#include <ostream>

/*!! include 'format' !! 51 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::idgen {

///////////////////////////////////////////////////////////////////////////////
bool is_valid(Format constant) noexcept {
   switch (constant) {
      case Format::standard:
      case Format::canonical:
      case Format::tab:
      case Format::json:
      case Format::xml:
         return true;
      default:
         return false;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* format_name(Format constant) noexcept {
   switch (constant) {
      case Format::standard:  return "standard";
      case Format::canonical: return "canonical";
      case Format::tab:       return "tab";
      case Format::json:      return "json";
      case Format::xml:       return "xml";
      default:
         return "?";
   }
}

///////////////////////////////////////////////////////////////////////////////
std::array<const Format, 5> format_values() noexcept {
   return ::be::EnumTraits<Format>::values<>();
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, Format constant) {
   if (is_valid(constant)) {
      os << format_name(constant);
   } else {
      os << static_cast<I64>(static_cast<U8>(constant));
   }
   return os;
}

} // be::idgen


/* ######################### END OF GENERATED CODE ######################### */

