#include "Info_Receiver.hpp"
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <zlib.h>
#define RESET "\033[0m"
#define RED "\033[31m"
void printError(const std::string &);

int main(int argc, char *argv[]) {
  //   std::cout << "Number of arguments: " << argc << std::endl;

  std::string hostname = argv[1];

  uint16_t port = std::stoi(argv[2]);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  // if (inet_pton(AF_INET, hostname.c_str(), &serverAddr.sin_addr) <= 0) {
  //     printError("Invalid IP address");
  //     return 1;
  // }



  std::cout << "change" << std::endl;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    printError("Failed to create socket");
    return 1;
  }

  if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    printError("Bind failed with error code: " + std::to_string(errno));
    close(sock);
    return 1;
  }

  const int bufferSize = 65536 << 3; // (64 << 1) KB buffer
  std::vector<Bytef> receiveBuffer(bufferSize);

  while (true) {
    ssize_t receivedBytes = recv(sock, receiveBuffer.data(), bufferSize, 0);
    if (receivedBytes == -1) {
      std::cerr << "Failed to receive data" << std::endl;
      continue; // continue listening for new data
    }
    std::cout << "Received compressed data length: " << receivedBytes << " bytes"
              << std::endl;

    unsigned long destLen = bufferSize;
    std::vector<Bytef> decompressedData(bufferSize);
    int uncompressResult = uncompress(decompressedData.data(), &destLen,
                                      receiveBuffer.data(), receivedBytes);
    if (uncompressResult != Z_OK) {
      std::cerr << "Decompression failed with error code: " << uncompressResult
                << std::endl;
      continue; // continue listening for new data
    }

    std::string json_str(reinterpret_cast<char *>(decompressedData.data()),
                         destLen);

    std::size_t sizeInBytes = json_str.size();
    std::cout << "Depressed data length: " << sizeInBytes << std::endl;

    std::cout << json_str << std::endl;

    // Process the received data here

  }

  close(sock); // Close the socket when the loop is terminated

  return 0;
}


void printError(const std::string &error) {
  std::cerr << RED << error << RESET << std::endl;
}