#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <string>

// Extracts a single valid IPv4 address (optionally followed by a port) that is
// embedded anywhere in `str`.
//
// Returns true if a valid address was found, false otherwise.
// On success: outAddress holds the 32-bit value, and outPort holds the port
//             number, or -1 if no port was present.
// On failure: outAddress is set to 0 and outPort is set to -1.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);

#endif // EXTRACTOR_H
