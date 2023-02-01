<p align="center">
  <img width="475" height="184" src="https://raw.githubusercontent.com/TSprech/SerialTransferCpp/master/logocpp.png">
</p>

# SerialTransferCpp
[![Standard](https://img.shields.io/badge/C%2B%2B-17-C%2B%2B20?logo=c%2B%2B&color=00599C&style=flat)](https://isocpp.org/std/the-standard)
[![Test Status](https://img.shields.io/github/actions/workflow/status/TSprech/SerialTransferCpp/cmake.yml?label=Test&logo=githubactions&logoColor=white)](https://github.com/TSprech/SerialTransferCpp/actions)
[![Coverage](https://img.shields.io/coveralls/github/TSprech/SerialTransferCpp?label=Coverage&logo=coveralls&logoColor=white)](https://coveralls.io/github/TSprech/SerialTransferCpp)
[![License](https://img.shields.io/github/license/TSprech/SerialTransferCpp?label=License&color=A41E34)](https://opensource.org/licenses/MIT)

## Sections
- [SerialTransferCpp](#serialtransfercpp)
  - [Sections](#sections)
    - [Acknowledgement](#acknowledgement)
  - [About](#about)
  - [How To Use](#how-to-use)
    - [Required Files](#required-files)
  - [Packet Anatomy](#packet-anatomy)
  - [Notes](#notes)

[//]: # ([![GitHub version]&#40;https://badge.fury.io/gh/PowerBroker2%2FSerialTransfer.svg&#41;]&#40;https://badge.fury.io/gh/PowerBroker2%2FSerialTransfer&#41;)
[//]: # ([![Download]&#40;https://img.shields.io/badge/Download%20%20-latest-blue.svg&#41;]&#40;https://raw.githubusercontent.com/TSprech/Micro-String-Format/main/include/usf/usf.hpp&#41;)

### Acknowledgement
Special thanks to PowerBroker2's [original library](https://github.com/PowerBroker2/SerialTransfer) for Arduino. This fork is just a simple modification which lightly builds off of it.

## About
SerialTransferCpp is an easy to use C++ based library for transferring packetized data fast and reliably via **Serial interfaces**. Communicate between your embedded device and computer easily.

[A mirror library is also available for Python!](https://github.com/PowerBroker2/pySerialTransfer) _This library is compatible with the same Python library the original SerialTransfer uses._

This library:
- Is non blocking
- Uses packet delimiters
- Uses consistent overhead byte stuffing
- Uses CRC-8 (Polynomial 0x9B with lookup table)
- Allows the use of dynamically sized packets (packets can have payload lengths anywhere from 1 to 254 bytes)
  - **can transfer bytes, ints, floats, structs, even large files like JPEGs and CSVs!!**
- Supports user-specified callback functions
- Uses standard library calls are removes all Arduino dependencies
- Can be run in a desktop environment for testing
- Is CMake based for easy integration

## How To Use
All of the library usage is the same as the original SerialTransfer, with two exceptions:

- The SerialTransferCpp object resides in the `stcpp` namespace, so the SerialTransfer object is created using `stcpp::SerialTransfer object`
    - After that, the operations are exactly the same except for...
- The `begin` method
    - The new method takes in a `SerialConfig` object which describes how the library will interface with the hardware
    - The `SerialConfig` object has 3 fields:
        - `put_c` function signature `void(int)` which takes in a byte of data and outputs it to another device (embedded → desktop)
        - `get_c` function signature `int(void)` which returns a byte of data that was input from another device (desktop → embedded)
        - `peek_c` function signature `bool(void)` which returns `true` if data is available to read, `false` if there is no waiting data (desktop → embedded\<check if embedded buffer has data ready\>)
    - Example of `SerialConfig` object that utilizes stdio:
      ```cpp
      inline SerialConfig stdio_config = {
                 .put_c = [](auto ch) { putc(ch, stdout); },
                 .get_c = []() -> int { return getc(stdin); },
                 .peek_c = []() { auto c = fgetc(stdin);
                                  ungetc(c, stdin);
                                  return c != EOF; }};
      ```
### Required Files
The only header/source files required for the library are:
- Millis.hpp
  - Modify this file to use your native millisecond function (replacement for Arduino millis)
- SerialConfig.hpp
  - The file which contains the struct to configure the library's serial interface
- PacketCpp.hpp
- PacketCpp.cpp
- PacketCRCCpp.hpp
- SerialTransferCpp.hpp
- SerialTransferCpp.cpp

_The other files are the Arduino compatible files for testing and reference, they are NOT required for library functionality_

## Packet Anatomy
```
01111110 00000000 11111111 00000000 00000000 00000000 ... 00000000 10000001
|      | |      | |      | |      | |      | |      | | | |      | |______|__Stop byte
|      | |      | |      | |      | |      | |      | | | |______|___________8-bit CRC
|      | |      | |      | |      | |      | |      | |_|____________________Rest of payload
|      | |      | |      | |      | |      | |______|________________________2nd payload byte
|      | |      | |      | |      | |______|_________________________________1st payload byte
|      | |      | |      | |______|__________________________________________# of payload bytes
|      | |      | |______|___________________________________________________COBS Overhead byte
|      | |______|____________________________________________________________Packet ID (0 by default)
|______|_____________________________________________________________________Start byte (constant)
```

See the [example sketches](https://github.com/PowerBroker2/SerialTransfer/tree/master/examples) in the original SerialTransfer library repo.

## Notes
Currently there is no support for the I2C and SPI version of the library, as the primary focus of this library is for `embedded → Python`, however pull requests are welcome.