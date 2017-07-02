#include <be/core/console.hpp>
#include <be/core/id.hpp>
#include <be/core/lifecycle.hpp>
#include <iostream>
#include <sstream>

using namespace be;

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
   // TODO Use KeywordParser to parse color names as well as numbers

   be::CoreInitLifecycle init;

   LogColor fg = LogColor::gray;
   LogColor bg = LogColor::black;

   if (argc > 1) {
      std::stringstream ss;
      ss << argv[1];

      U16 c;
      ss >> c;

      fg = static_cast<LogColor>(c);
   }

   if (argc > 2) {
      std::stringstream ss;
      ss << argv[2];

      U16 c;
      ss >> c;

      bg = static_cast<LogColor>(c);
   }

   set_console_color_at_exit({ fg, bg });
   return 0;
}
