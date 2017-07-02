#include "mode.hpp"

/*!! include 'idgen/mode' !! 47 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::idgen {

///////////////////////////////////////////////////////////////////////////////
bool is_valid(Mode constant) noexcept {
   switch (constant) {
      case Mode::fnv0:
      case Mode::fnv1:
      case Mode::fnv1a:
         return true;
      default:
         return false;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* mode_name(Mode constant) noexcept {
   switch (constant) {
      case Mode::fnv0:  return "fnv0";
      case Mode::fnv1:  return "fnv1";
      case Mode::fnv1a: return "fnv1a";
      default:
         return "?";
   }
}

///////////////////////////////////////////////////////////////////////////////
std::array<const Mode, 3> mode_values() noexcept {
   return ::be::EnumTraits<Mode>::values<>();
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, Mode constant) {
   if (is_valid(constant)) {
      os << mode_name(constant);
   } else {
      os << static_cast<I64>(static_cast<U8>(constant));
   }
   return os;
}

} // be::idgen


/* ######################### END OF GENERATED CODE ######################### */

