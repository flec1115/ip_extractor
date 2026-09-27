#include <iostream>
#include <string>

#include "extractor.h"

int main() {
  std::string line;

  while (true) {
    std::cout << "Enter text (or END to quit): ";

    if (!std::getline(std::cin, line)) {
      // End-of-input (e.g. Ctrl-D) is treated like END.
      std::cout << "\nProgram terminated." << std::endl;
      break;
    }

    if (line == "END") {
      std::cout << "Program terminated." << std::endl;
      break;
    }

    unsigned long address = 0;
    int port = -1;

    if (extractIPv4(line, address, port)) {
      const unsigned long a = (address >> 24) & 0xFFUL;
      const unsigned long b = (address >> 16) & 0xFFUL;
      const unsigned long c = (address >> 8) & 0xFFUL;
      const unsigned long d = address & 0xFFUL;

      // the below was initially created by AI and redone by myself
      std::cout << "Extracted IPv4 address: " << a << '.' << b << '.' << c
                << '.' << d << " (decimal value: " << address << ", port: ";
      if (port == -1) {
        std::cout << "none";
      } else {
        std::cout << port;
      }
      std::cout << ")" << std::endl;
    } else {
      std::cout << "No valid IPv4 address found." << std::endl;
    }
  }

  return 0;
}
