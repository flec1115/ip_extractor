#include "extractor.h"

#include <cctype>

// -----------------------------------------------------------------------------
// A "token character" is the only kind of character that can ever be part of a
// candidate token: a digit, a period, or a colon. Everything else is garbage
// and acts as a delimiter between candidate tokens.
// -----------------------------------------------------------------------------
static bool isTokenChar(char c) {
    return std::isdigit(static_cast<unsigned char>(c)) || c == '.' || c == ':';
}

// Parse a single octet from s[pos, end).
//
// Rules:
//   * 1 to 3 digits
//   * value in [0, 255]
//   * no leading zero unless the value is exactly 0 (so "0" is fine, but "00"
//     and "01" are rejected)
//
// On success, advances `pos` past the digits consumed and writes the value.
// All digit accumulation is done by hand (no atoi/strtol/etc.).
static bool parseOctet(const std::string& s, size_t& pos, size_t end,
                       unsigned int& outVal) {
    if (pos >= end || !std::isdigit(static_cast<unsigned char>(s[pos]))) {
        return false;
    }

    const size_t firstDigit = pos;
    unsigned int val = 0;
    int count = 0;

    while (pos < end && count < 3 &&
           std::isdigit(static_cast<unsigned char>(s[pos]))) {
        val = val * 10 + static_cast<unsigned int>(s[pos] - '0');
        ++pos;
        ++count;
    }

    // Disallowed leading zero: more than one digit and the first is '0'.
    if (count > 1 && s[firstDigit] == '0') {
        return false;
    }
    if (val > 255) {
        return false;
    }

    outVal = val;
    return true;
}

// Parse a port from s[pos, end).
//
// Rules:
//   * 1 to 5 digits
//   * value in [0, 65535]
//   * same leading-zero rule as an octet
//
// On success, advances `pos` past the digits consumed and writes the value.
static bool parsePort(const std::string& s, size_t& pos, size_t end,
                      int& outVal) {
    if (pos >= end || !std::isdigit(static_cast<unsigned char>(s[pos]))) {
        return false;
    }

    const size_t firstDigit = pos;
    unsigned long val = 0;
    int count = 0;

    while (pos < end && count < 5 &&
           std::isdigit(static_cast<unsigned char>(s[pos]))) {
        val = val * 10 + static_cast<unsigned long>(s[pos] - '0');
        ++pos;
        ++count;
    }

    if (count > 1 && s[firstDigit] == '0') {
        return false;
    }
    if (val > 65535) {
        return false;
    }

    outVal = static_cast<int>(val);
    return true;
}

// Validate the entire candidate token in s[start, end) against the grammar:
//     octet '.' octet '.' octet '.' octet ( ':' port )?
//
// The whole run must be consumed exactly -- no partial matches, no truncating
// to find a valid piece inside a longer run. Any leftover character (a stray
// period, a second colon, extra digits, etc.) rejects the token.
static bool validateToken(const std::string& s, size_t start, size_t end,
                          unsigned long& outAddr, int& outPort) {
    size_t pos = start;
    unsigned int octets[4];

    for (int k = 0; k < 4; ++k) {
        if (!parseOctet(s, pos, end, octets[k])) {
            return false;
        }
        if (k < 3) {
            // A '.' must immediately separate the first four octets.
            if (pos >= end || s[pos] != '.') {
                return false;
            }
            ++pos;
        }
    }

    int port = -1;
    if (pos < end) {
        // The only thing allowed after the fourth octet is ":port".
        if (s[pos] != ':') {
            return false;
        }
        ++pos;
        if (!parsePort(s, pos, end, port)) {
            return false;
        }
    }

    // The token must be fully consumed; anything left over is a rejection.
    if (pos != end) {
        return false;
    }

    outAddr = (static_cast<unsigned long>(octets[0]) << 24) |
              (static_cast<unsigned long>(octets[1]) << 16) |
              (static_cast<unsigned long>(octets[2]) << 8) |
              (static_cast<unsigned long>(octets[3]));
    outPort = port;
    return true;
}

bool extractIPv4(const std::string& str, unsigned long& outAddress,
                 int& outPort) {
    outAddress = 0;
    outPort = -1;

    const size_t n = str.size();
    size_t i = 0;

    while (i < n) {
        // Skip garbage until the start of a run of token characters.
        if (!isTokenChar(str[i])) {
            ++i;
            continue;
        }

        // Capture the maximal run of token characters. The whole run is the
        // candidate token -- we never truncate it to find a valid piece.
        const size_t runStart = i;
        while (i < n && isTokenChar(str[i])) {
            ++i;
        }
        const size_t runEnd = i;

        unsigned long addr = 0;
        int port = -1;
        if (validateToken(str, runStart, runEnd, addr, port)) {
            outAddress = addr;
            outPort = port;
            return true;
        }
        // Otherwise fall through and keep scanning after this run.
    }

    outAddress = 0;
    outPort = -1;
    return false;
}
