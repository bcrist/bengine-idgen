#include "type.hpp"
#include <ostream>

/*!! include 'idgen/type' !! 51 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::idgen {

///////////////////////////////////////////////////////////////////////////////
bool is_valid(Type constant) noexcept {
   switch (constant) {
      case Type::signed_dec:
      case Type::unsigned_dec:
      case Type::hex:
      case Type::base64:
      case Type::hash:
         return true;
      default:
         return false;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* type_name(Type constant) noexcept {
   switch (constant) {
      case Type::signed_dec:   return "signed_dec";
      case Type::unsigned_dec: return "unsigned_dec";
      case Type::hex:          return "hex";
      case Type::base64:       return "base64";
      case Type::hash:         return "hash";
      default:
         return "?";
   }
}

///////////////////////////////////////////////////////////////////////////////
std::array<const Type, 5> type_values() noexcept {
   return ::be::EnumTraits<Type>::values<>();
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, Type constant) {
   if (is_valid(constant)) {
      os << type_name(constant);
   } else {
      os << static_cast<I64>(static_cast<U8>(constant));
   }
   return os;
}

} // be::idgen


/* ######################### END OF GENERATED CODE ######################### */

