//
// Created by treys on 2023/01/29 18:13:32.
//

#ifndef INTERFACECONFIG_HPP
#define INTERFACECONFIG_HPP

#include <functional>
#include <string_view>

struct SerialConfig {
  std::function<void(int)> put_c;   // Writes a single byte value to the serial output
  std::function<int(void)> get_c;   // Reads a single bytes value from the serial input and clears it from the receive buffer
  std::function<bool(void)> peek_c;  // Checks to see if any bytes are available to read from serial input
};

inline SerialConfig stdio_config = {
    .put_c = [](auto ch) { putc(ch, stdout); },
    .get_c = []() -> int { return getc(stdin); },
    .peek_c = []() { auto c = fgetc(stdin);
                     ungetc(c, stdin);
                     return c != EOF; }};

inline void puts(SerialConfig& sconf, std::string_view str) {
  for (auto ch : str) {
    sconf.put_c(ch);
  }
}

#endif  //INTERFACECONFIG_HPP
