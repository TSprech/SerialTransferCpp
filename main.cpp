//
// Created by TSprech on 2023/01/29 10:06:16.
//

#include <iostream>

#include "SerialTransferCpp.h"

SerialTransfer myTransfer;

struct __attribute__((packed)) STRUCT {
  char z;
  float y;
} testStruct;

char arr[] = "hello";

int main() {
  myTransfer.begin(stdout);

  testStruct.z = '$';
  testStruct.y = 4.5;

  // use this variable to keep track of how many
  // bytes we're stuffing in the transmit buffer
  uint16_t sendSize = 0;

  ///////////////////////////////////////// Stuff buffer with struct
  sendSize = myTransfer.txObj(testStruct, sendSize);

  ///////////////////////////////////////// Stuff buffer with array
  sendSize = myTransfer.txObj(arr, sendSize);

  ///////////////////////////////////////// Send buffer
  myTransfer.sendData(sendSize);
  return 0;
}