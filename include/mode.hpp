#pragma once
#ifndef BE_IDGEN_MODE_HPP_
#define BE_IDGEN_MODE_HPP_

#include <be/core/enum_traits.hpp>

/*!! include 'mode' !! 50 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::idgen {

///////////////////////////////////////////////////////////////////////////////
enum class Mode : U8 {
   fnv0 = 0,
   fnv1,
   fnv1a
};

bool is_valid(Mode constant) noexcept;
const char* mode_name(Mode constant) noexcept;
std::array<const Mode, 3> mode_values() noexcept;
std::ostream& operator<<(std::ostream& os, Mode constant);

} // be::idgen

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct EnumTraits<::be::idgen::Mode> {
   using type = ::be::idgen::Mode;
   using underlying_type = typename std::underlying_type<type>::type;

   static constexpr std::size_t count = 3;

   static bool is_valid(type value) {
      return ::be::idgen::is_valid(value);
   }

   static const char* name(type value) {
      return ::be::idgen::mode_name(value);
   }

   template <typename C = std::array<const type, count>>
   static C values() {
      return {
         ::be::idgen::Mode::fnv0,
         ::be::idgen::Mode::fnv1,
         ::be::idgen::Mode::fnv1a,
      };
   }
};

} // be

/* ######################### END OF GENERATED CODE ######################### */


#endif
