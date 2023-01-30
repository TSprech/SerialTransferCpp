<p align="center">
  <img width="475" height="184" src="https://raw.githubusercontent.com/TSprech/SerialTransferCpp/master/logocpp.png">
</p>

# SerialTransferCpp
[![Standard](https://img.shields.io/badge/C%2B%2B-17-C%2B%2B20?logo=c%2B%2B&color=00599C&style=flat)](https://isocpp.org/std/the-standard)
[![Test Status](https://img.shields.io/github/actions/workflow/status/TSprech/SerialTransferCpp/cmake.yml?label=Test&logo=githubactions&logoColor=white)](https://github.com/TSprech/SerialTransferCpp/actions)
[![Coverage](https://img.shields.io/coveralls/github/TSprech/SerialTransferCpp?label=Coverage&logo=coveralls&logoColor=white)](https://coveralls.io/github/TSprech/SerialTransferCpp)
[![License](https://img.shields.io/github/license/TSprech/SerialTransferCpp?label=License&color=A41E34)](https://opensource.org/licenses/MIT)

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
- Supports user-specified callback functions
- **can transfer bytes, ints, floats, structs, even large files like JPEGs and CSVs!!**

## Packet Anatomy:
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

## ***How To Use:***

See the [example sketches](https://github.com/PowerBroker2/SerialTransfer/tree/master/examples)

- datum = tx/rx a single object
- data = tx/rx multiple objects

### ***NOTE:***
Currently there is no support for the I2C and SPI version of the library, as the primary focus of this library is for embedded → Python, however pull requests are welcome.