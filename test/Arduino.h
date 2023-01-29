//
// Created by treys on 2023/01/29 10:17:40.
//

#ifndef SERIALTRANSFERCPP_ARDUINO_H
#define SERIALTRANSFERCPP_ARDUINO_H

#include <cmath>
#include <cstdint>
#include <deque>
#include <string_view>
#include <cstring>

#define F

#define HEX 0

using byte = uint8_t;

class Stream {
 public:
  inline size_t write(uint8_t* buf, size_t len) {
    for (size_t index = 0; index < len; ++index) {
      tx_buffer.push_back(buf[index]);
    }
    return len;
  }

  inline size_t available() const {
    return rx_buffer.size();
  }

  inline int read() {
    if (!available()) return -1;
    auto value = rx_buffer.front();
    rx_buffer.pop_front();
    return value;
  }

  inline size_t print(std::string_view str, int format = 0) {
    for (auto ch : str) {
      tx_buffer.push_back(ch);
    }
    return str.length();
  }

  inline size_t print(uint8_t ch, int format = 0) {
    tx_buffer.push_back(ch);
    return 1;
  }

  //  size_t print(const std::string &);
  //  size_t print(const char[]);
  //  size_t print(char);
  //  size_t print(unsigned char, int = DEC);
  //  size_t print(int, int = DEC);
  //  size_t print(unsigned int, int = DEC);
  //  size_t print(long, int = DEC);
  //  size_t print(unsigned long, int = DEC);
  //  size_t print(double, int = 2);
  //  size_t print(const Printable&);
  //
  //  size_t println(const std::string &s);
  //  size_t println(const char[]);
  //  size_t println(char);
  //  size_t println(unsigned char, int = DEC);
  //  size_t println(int, int = DEC);
  //  size_t println(unsigned int, int = DEC);
  //  size_t println(long, int = DEC);
  //  size_t println(unsigned long, int = DEC);
  //  size_t println(double, int = 2);
  //  size_t println(const Printable&);
  //  size_t println(void);

  inline size_t println(std::string_view str) {
    auto len = print(str);
    print("\n");
    return len + 1;
  }

  inline size_t println(uint8_t str) {
    auto len = print(str);
    print("\n");
    return len + 1;
  }

  inline size_t println() {
    print("\n");
    return 1;
  }


// private:
  std::deque<char> tx_buffer = {};
  std::deque<char> rx_buffer = {};
};

inline Stream Serial;

inline uint32_t millis() {
  // Put your millis implimentation here
  return 0;
}

#endif  // SERIALTRANSFERCPP_ARDUINO_H
