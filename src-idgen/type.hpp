#pragma once
#ifndef BE_IDGEN_TYPE_HPP_
#define BE_IDGEN_TYPE_HPP_

#include <be/core/enum_traits.hpp>

/*!! include 'idgen/type' !! 54 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::idgen {

///////////////////////////////////////////////////////////////////////////////
enum class Type : U8 {
   signed_dec = 0,
   unsigned_dec,
   hex,
   base64,
   hash
};

bool is_valid(Type constant) noexcept;
const char* type_name(Type constant) noexcept;
std::array<const Type, 5> type_values() noexcept;
std::ostream& operator<<(std::ostream& os, Type constant);

} // be::idgen

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct EnumTraits<::be::idgen::Type> {
   using type = ::be::idgen::Type;
   using underlying_type = typename std::underlying_type<type>::type;

   static constexpr std::size_t count = 5;

   static bool is_valid(type value) {
      return ::be::idgen::is_valid(value);
   }

   static const char* name(type value) {
      return ::be::idgen::type_name(value);
   }

   template <typename C = std::array<const type, count>>
   static C values() {
      return {
         ::be::idgen::Type::signed_dec,
         ::be::idgen::Type::unsigned_dec,
         ::be::idgen::Type::hex,
         ::be::idgen::Type::base64,
         ::be::idgen::Type::hash,
      };
   }
};

} // be

/* ######################### END OF GENERATED CODE ######################### */


#endif
