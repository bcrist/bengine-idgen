#include "Windows.h"
#include <string>

std::wstring parse_command_line(const std::wstring& str);

// Windows Elevated Do:
// Forwards the entire command line except for the first parameter (the wedo.exe module name) to CreateProcess.
// When linked with /MANIFESTUAC:"level='requireAdministrator'" this works as a poor man's `sudo` for Windows.
int main() {
   std::wstring args = parse_command_line(::GetCommandLineW());

   if (args.empty()) {
      return 0;
   }

   ::STARTUPINFOW si; 
   ::PROCESS_INFORMATION pi;
   std::memset(&si, 0, sizeof(si)); 
   std::memset(&pi, 0, sizeof(pi)); 
   si.cb = sizeof(si); 

   ::DWORD exit;

   if (::CreateProcessW(NULL, const_cast<::LPWSTR>(args.data()), NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) != FALSE) {
      exit = ::WaitForSingleObject(pi.hProcess, INFINITE);
   } else {
      exit = ::GetLastError();
   }

   ::CloseHandle(pi.hProcess);
   ::CloseHandle(pi.hThread);

   return exit;
}

std::wstring parse_command_line(const std::wstring& str) {
   /*2n backslashes followed by a quotation mark produce n backslashes followed by a quotation mark.
   (2n) + 1 backslashes followed by a quotation mark again produce n backslashes followed by a quotation mark.
   n backslashes not followed by a quotation mark simply produce n backslashes.
   Important  
   CommandLineToArgvW treats whitespace outside of quotation marks as argument delimiters. However, if lpCmdLine starts with any amount of whitespace, CommandLineToArgvW will consider the first argument to be an empty string. Excess whitespace at the end of lpCmdLine is ignored.*/

   enum State {
      init,
      self,
      quoted_self,
      target_ws
   } state = init;
   int slashes = 0;
   std::wstring::const_iterator cursor = str.end();

   for (auto it = str.begin(), end = str.end(); it != end; ++it) {
      wchar_t c = *it;

      if (state == init) {
         if (c == '"') {
            state = quoted_self;
            continue;
         } else if (c <= ' ') {
            state = target_ws;
            continue;
         } else if (c == '\\') {
            ++slashes;
         }
         state = self;

      } else if (state == self) {
         if (c == '"' && (slashes & 1) == 0) {
            slashes = 0;
            state = quoted_self;
         } else if (c <= ' ') {
            slashes = 0;
            state = target_ws;
         } else if (c == '\\') {
            ++slashes;
         } else {
            slashes = 0;
         }

      } else if (state == quoted_self) {
         if (c == '"' && (slashes & 1) == 0) {
            slashes = 0;
            state = self;
         } else if (c == '\\') {
            ++slashes;
         } else {
            slashes = 0;
         }

      } else if (state == target_ws) {
         if (c <= ' ') {
            continue;
         } else {
            cursor = it;
            break;
         }
      }
   }

   return std::wstring(cursor, str.end());
}
