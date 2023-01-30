//
// Created by treys on 2023/01/29 14:52:40.
//

#include "Arduino.h"
#include "SerialTransfer.h"
#include "SerialTransferCpp.h"
#include "gtest/gtest.h"

namespace {
  void print_captured_data(std::string captured) { // Used for generating buffers that can be used for the RX tests
    std::cout << "\nData:\n";
    for (auto ch : captured) {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << ",0x" << static_cast<int>(static_cast<uint8_t>(ch));
    }
    std::cout << "\n";
  }

  TEST(SerialTransferCpp, uart_tx_data) {
    testing::internal::CaptureStdout();

    SerialTransfer myTransfer;
    stcpp::SerialTransfer cppTransfer;

    struct __attribute__((packed)) STRUCT {
      char z;
      float y;
    } testStruct;

    char arr[] = "hello";

    myTransfer.begin(Serial);

    testStruct.z = '$';
    testStruct.y = 4.5;

    uint16_t sendSize = 0;
    sendSize = myTransfer.txObj(testStruct, sendSize);
    sendSize = myTransfer.txObj(arr, sendSize);
    myTransfer.sendData(sendSize);

    cppTransfer.begin(stdio_config);

    sendSize = 0;
    sendSize = cppTransfer.txObj(testStruct, sendSize);
    sendSize = cppTransfer.txObj(arr, sendSize);
    cppTransfer.sendData(sendSize);

    auto stdout_captured = testing::internal::GetCapturedStdout();
    print_captured_data(stdout_captured);
    EXPECT_EQ(Serial.tx_buffer.size(), stdout_captured.size());
    for (int i = 0; i < stdout_captured.size(); ++i) {
      EXPECT_EQ(Serial.tx_buffer[i], stdout_captured[i]);
    }
  }

  TEST(SerialTransferCpp, uart_tx_datum) {
    testing::internal::CaptureStdout();

    SerialTransfer myTransfer;
    stcpp::SerialTransfer cppTransfer;

    struct __attribute__((packed)) STRUCT {
      char z;
      float y;
    } testStruct;

    myTransfer.begin(Serial);

    testStruct.z = '$';
    testStruct.y = 4.5;

    myTransfer.sendDatum(testStruct);

    cppTransfer.begin(stdio_config);

    cppTransfer.sendDatum(testStruct);

    auto stdout_captured = testing::internal::GetCapturedStdout();
    print_captured_data(stdout_captured);
    EXPECT_EQ(Serial.tx_buffer.size(), stdout_captured.size());
    for (int i = 0; i < stdout_captured.size(); ++i) {
      EXPECT_EQ(Serial.tx_buffer[i], stdout_captured[i]);
    }
  }

  TEST(SerialTransferCpp, uart_tx_file) {
    testing::internal::CaptureStdout();

    SerialTransfer myTransfer;
    stcpp::SerialTransfer cppTransfer;

    const int fileSize = 2001;
    char file[fileSize] = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestib";
    char fileName[] = "test.txt";

    myTransfer.begin(Serial);

    myTransfer.sendDatum(fileName);  // Send filename

    uint16_t numPackets = fileSize / (MAX_PACKET_SIZE - 2);  // Reserve two bytes for current file index

    if (fileSize % MAX_PACKET_SIZE)  // Add an extra transmission if needed
      numPackets++;

    for (uint16_t i = 0; i < numPackets; i++)  // Send all data within the file across multiple packets
    {
      uint16_t fileIndex = i * MAX_PACKET_SIZE;  // Determine the current file index
      uint8_t dataLen = MAX_PACKET_SIZE - 2;

      if ((fileIndex + (MAX_PACKET_SIZE - 2)) > fileSize)  // Determine data length for the last packet if file length is not an exact multiple of MAX_PACKET_SIZE-2
        dataLen = fileSize - fileIndex;

      uint8_t sendSize = myTransfer.txObj(fileIndex);                   // Stuff the current file index
      sendSize = myTransfer.txObj(file[fileIndex], sendSize, dataLen);  // Stuff the current file data

      myTransfer.sendData(sendSize, 1);  // Send the current file index and data
    }

    cppTransfer.begin(stdio_config);

    cppTransfer.sendDatum(fileName);  // Send filename

    numPackets = fileSize / (MAX_PACKET_SIZE - 2);  // Reserve two bytes for current file index

    if (fileSize % MAX_PACKET_SIZE)  // Add an extra transmission if needed
      numPackets++;

    for (uint16_t i = 0; i < numPackets; i++)  // Send all data within the file across multiple packets
    {
      uint16_t fileIndex = i * MAX_PACKET_SIZE;  // Determine the current file index
      uint8_t dataLen = MAX_PACKET_SIZE - 2;

      if ((fileIndex + (MAX_PACKET_SIZE - 2)) > fileSize)  // Determine data length for the last packet if file length is not an exact multiple of MAX_PACKET_SIZE-2
        dataLen = fileSize - fileIndex;

      uint8_t sendSize = cppTransfer.txObj(fileIndex);                   // Stuff the current file index
      sendSize = cppTransfer.txObj(file[fileIndex], sendSize, dataLen);  // Stuff the current file data

      cppTransfer.sendData(sendSize, 1);  // Send the current file index and data
    }

    auto stdout_captured = testing::internal::GetCapturedStdout();
    print_captured_data(stdout_captured);
    EXPECT_EQ(Serial.tx_buffer.size(), stdout_captured.size());
    for (int i = 0; i < stdout_captured.size(); ++i) {
      EXPECT_EQ(Serial.tx_buffer[i], stdout_captured[i]);
    }
  }
}  // namespace
