#pragma once
#ifndef BE_SIZEOF_SIZE_OF_TYPE_HPP_
#define BE_SIZEOF_SIZE_OF_TYPE_HPP_

#include <be/core/console.hpp>
#include <iostream>

namespace be {

///////////////////////////////////////////////////////////////////////////////
struct SizeOfType {
   std::size_t preview_chars = 32;
   std::size_t bits_per_preview_char = 1;
   std::size_t preview_char_grouping = sizeof(int*);

   template <typename T>
   void size(S name = type_name<T>(), LogColor name_color = LogColor::yellow, bool show_type_name_alias = false);
   void separator(char fill_char = ' ');
};

template <typename T>
void SizeOfType::size(S name, LogColor name_color, bool show_type_name_alias) {
   using namespace color;

   std::size_t n = sizeof(T);
   std::size_t a = alignof(T);

   std::size_t padded_n = 0;
   while (padded_n < n) padded_n += a;

   if (preview_chars > 0) {
      std::size_t bytes = preview_chars * bits_per_preview_char;

      if (n > bytes && a > bytes) {
         std::cout << cyan << " ...";
      } else if (n > bytes) {
         std::cout << blue << " ...";
      } else if (a > bytes) {
         std::cout << green << " ...";
      } else if (padded_n > bytes) {
         std::cout << gray << " ...";
      } else {
         std::cout << "    ";
      }

      std::size_t initial = 0;
      while (initial < bytes) initial += bits_per_preview_char;

      for (std::size_t i = initial; i > 0; i -= bits_per_preview_char) {
         if (preview_char_grouping > bits_per_preview_char) {
            for (std::size_t j = 0; j < bits_per_preview_char; ++j) {
               if ((i - j) % preview_char_grouping == 0) {
                  std::cout << dark_gray << '|';
                  break;
               }
            }
         }

         char ch = ' ';
         bool b = false;
         bool g = false;

         if (i <= n) {
            ch = '#';
            b = true;
         } else if (i - bits_per_preview_char + 1 <= n) {
            ch = ':';
            b = true;
         } else if (i - bits_per_preview_char + 1 <= padded_n) {
            ch = '_';
         }

         if (i - bits_per_preview_char + 1 <= a) {
            g = true;
         }

         if (b && g) std::cout << cyan;
         else if (b) std::cout << blue;
         else if (g) std::cout << green;
         else std::cout << gray;

         std::cout << ch;
      }
      if (preview_char_grouping > bits_per_preview_char) {
         std::cout << dark_gray << '|';
      }
   }

   std::cout << std::right << std::setw(5);

   if (a == n) {
      std::cout << cyan << n << std::left << "    ";
   } else {
      std::cout << blue << n << std::left << green << " " << a << " ";
      if (a < 10) std::cout << ' ';
   }
   std::cout << setcolor(name_color) << name;
   if (show_type_name_alias) {
      SV tname = type_name<T>();
      if (name != tname) {
         std::cout << dark_gray << " = " << tname;
      }
   }
   std::cout << gray << std::endl;
}

} // be

#endif
