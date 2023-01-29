//
// Created by TSprech on 2023/01/29 10:06:16.
//

//#include <iostream>
//
//#include "SerialTransferCpp.h"
//#include "SerialTransfer.h"
//#include "Arduino.h"
//
//SerialTransfer myTransfer;
//stcpp::SerialTransfer cppTransfer;
//
//struct __attribute__((packed)) STRUCT {
//  char z;
//  float y;
//} testStruct;
//
//char arr[] = "hello";
//
//int main() {
//  myTransfer.begin(Serial);
//
//  testStruct.z = '$';
//  testStruct.y = 4.5;
//
//  uint16_t sendSize = 0;
//  sendSize = myTransfer.txObj(testStruct, sendSize);
//  sendSize = myTransfer.txObj(arr, sendSize);
//  myTransfer.sendData(sendSize);
//
//  for (auto ch : Serial.tx_buffer) {
//    std::cout << ch;
//  }
//
//  std::cout << "\n------------------------------------------------------------------------\n";
//
//  cppTransfer.begin(stdout);
//
//  sendSize = 0;
//  sendSize = cppTransfer.txObj(testStruct, sendSize);
//  sendSize = cppTransfer.txObj(arr, sendSize);
//  cppTransfer.sendData(sendSize);
//  return 0;
//}