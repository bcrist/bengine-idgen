#include "size_of_type.hpp"

namespace be {

void SizeOfType::separator(char fill_char) {
   std::cout << color::dark_gray;
   if (preview_chars > 0) {
      size_t bytes = preview_chars * bits_per_preview_char;

      std::cout << S(4, fill_char);

      size_t initial = 0;
      while (initial < bytes) initial += bits_per_preview_char;

      for (size_t i = initial; i > 0; i -= bits_per_preview_char) {
         if (preview_char_grouping > bits_per_preview_char) {
            for (size_t j = 0; j < bits_per_preview_char; ++j) {
               if ((i - j) % preview_char_grouping == 0) {
                  std::cout << color::dark_gray << '|';
                  break;
               }
            }
         }

         std::cout << fill_char;
      }
      if (preview_char_grouping > bits_per_preview_char) {
         std::cout << color::dark_gray << '|';
      }
   }

   std::cout << S(40, fill_char);
   std::cout << std::endl;
}

} // be
