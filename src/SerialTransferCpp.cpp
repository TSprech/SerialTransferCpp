#include "SerialTransferCpp.h"

#include "millis.h"

namespace stcpp {
  int fpeekc(FILE* stream) {
    auto c = fgetc(stream);
    ungetc(c, stream);
    return c;
  }

  /*
 void SerialTransfer::begin(FILE* _port, configST configs)
 Description:
 ------------
  * Advanced initializer for the SerialTransfer Class
 Inputs:
 -------
  * const FILE* _port - Serial port to communicate over
  * const configST configs - Struct that holds config
  values for all possible initialization parameters
 Return:
 -------
  * void
*/
  void SerialTransfer::begin(FILE* _port, const configST configs) {
    port = _port;
    packet.begin(configs);
  }

  /*
 void SerialTransfer::begin(FILE* _port, const bool _debug, FILE* _debugPort)
 Description:
 ------------
  * Simple initializer for the SerialTransfer Class
 Inputs:
 -------
  * const FILE* _port - Serial port to communicate over
  * const bool _debug - Whether or not to print error messages
  * const FILE* _debugPort - Serial port to print error messages
 Return:
 -------
  * void
*/
  void SerialTransfer::begin(FILE* _port, const bool _debug, FILE* _debugPort, uint32_t _timeout) {
    port = _port;
    timeout = _timeout;
    packet.begin(_debug, _debugPort, _timeout);
  }

  /*
 uint8_t SerialTransfer::sendData(const uint16_t &messageLen, const uint8_t packetID)
 Description:
 ------------
  * Send a specified number of bytes in packetized form
 Inputs:
 -------
  * const uint16_t &messageLen - Number of values in txBuff
  to send as the payload in the next packet
  * const uint8_t packetID - The packet 8-bit identifier
 Return:
 -------
  * uint8_t numBytesIncl - Number of payload bytes included in packet
*/
  uint8_t SerialTransfer::sendData(const uint16_t& messageLen, const uint8_t packetID) {
    uint8_t numBytesIncl;

    numBytesIncl = packet.constructPacket(messageLen, packetID);
//    for (unsigned char i : packet.preamble) {
//      putc(i, stdout);
//    }
//    for (int i = 0; i < numBytesIncl; ++i) {
//      putc(packet.txBuff[i], stdout);
//    }
//
//    for (unsigned char i : packet.postamble) {
//      putc(i, stdout);
//    }

    fwrite(packet.preamble, sizeof(packet.preamble[0]), sizeof(packet.preamble), stdout);
    fwrite(packet.txBuff, sizeof(packet.txBuff[0]), numBytesIncl, stdout);
    fwrite(packet.postamble, sizeof(packet.postamble[0]), sizeof(packet.postamble), stdout);

    //	port->write(packet.preamble, sizeof(packet.preamble));
    //	port->write(packet.txBuff, numBytesIncl);
    //	port->write(packet.postamble, sizeof(packet.postamble));

    return numBytesIncl;
  }

  /*
 uint8_t SerialTransfer::available()
 Description:
 ------------
  * Parses incoming serial data, analyzes packet contents,
  and reports errors/successful packet reception
 Inputs:
 -------
  * void
 Return:
 -------
  * uint8_t bytesRead - Num bytes in RX buffer
*/
  uint8_t SerialTransfer::available() {
    bool valid = false;
    uint8_t recChar = 0xFF;

    if (fpeekc(stdin) != EOF) {
      valid = true;

      while (fpeekc(stdin) != EOF) {
        recChar = fgetc(stdin);

        bytesRead = packet.parse(recChar, valid);
        status = packet.status;

        if (status != CONTINUE) {
          if (status < 0)
            reset();

          break;
        }
      }
    } else {
      bytesRead = packet.parse(recChar, valid);
      status = packet.status;

      if (status < 0)
        reset();
    }

    return bytesRead;
  }

  /*
 bool SerialTransfer::tick()
 Description:
 ------------
  * Checks to see if any packets have been fully parsed. This
  is basically a wrapper around the method "available()" and
  is used primarily in conjunction with callbacks
 Inputs:
 -------
  * void
 Return:
 -------
  * bool - Whether or not a full packet has been parsed
*/
  bool SerialTransfer::tick() {
    if (available())
      return true;

    return false;
  }

  /*
 uint8_t SerialTransfer::currentPacketID()
 Description:
 ------------
  * Returns the ID of the last parsed packet
 Inputs:
 -------
  * void
 Return:
 -------
  * uint8_t - ID of the last parsed packet
*/
  uint8_t SerialTransfer::currentPacketID() {
    return packet.currentPacketID();
  }

  /*
 void SerialTransfer::reset()
 Description:
 ------------
  * Clears out the tx, and rx buffers, plus resets
  the "bytes read" variable, finite state machine, etc
 Inputs:
 -------
  * void
 Return:
 -------
  * void
*/
  void SerialTransfer::reset() {
    while (fpeekc(stdin) != EOF)
      fgetc(stdin);

    packet.reset();
    status = packet.status;
  }
}  // namespace stcpp
