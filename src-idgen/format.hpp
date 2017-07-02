#pragma once
#ifndef BE_IDGEN_FORMAT_HPP_
#define BE_IDGEN_FORMAT_HPP_

#include <be/core/enum_traits.hpp>

/*!! include 'idgen/format' !! 54 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::idgen {

///////////////////////////////////////////////////////////////////////////////
enum class Format : U8 {
   standard = 0,
   canonical,
   tab,
   json,
   xml
};

bool is_valid(Format constant) noexcept;
const char* format_name(Format constant) noexcept;
std::array<const Format, 5> format_values() noexcept;
std::ostream& operator<<(std::ostream& os, Format constant);

} // be::idgen

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct EnumTraits<::be::idgen::Format> {
   using type = ::be::idgen::Format;
   using underlying_type = typename std::underlying_type<type>::type;

   static constexpr std::size_t count = 5;

   static bool is_valid(type value) {
      return ::be::idgen::is_valid(value);
   }

   static const char* name(type value) {
      return ::be::idgen::format_name(value);
   }

   template <typename C = std::array<const type, count>>
   static C values() {
      return {
         ::be::idgen::Format::standard,
         ::be::idgen::Format::canonical,
         ::be::idgen::Format::tab,
         ::be::idgen::Format::json,
         ::be::idgen::Format::xml,
      };
   }
};

} // be

/* ######################### END OF GENERATED CODE ######################### */


#endif
