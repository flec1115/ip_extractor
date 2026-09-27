#include <iostream>
#include <string>

#include "extractor.h"

// ---------------------------------------------------------------------------
// Minimal self-contained test harness (no external framework).
// ---------------------------------------------------------------------------
static int g_passed = 0;
static int g_failed = 0;

static unsigned long makeAddr(unsigned int a, unsigned int b, unsigned int c,
                              unsigned int d) {
    return (static_cast<unsigned long>(a) << 24) |
           (static_cast<unsigned long>(b) << 16) |
           (static_cast<unsigned long>(c) << 8) |
           (static_cast<unsigned long>(d));
}

// Expect a successful extraction with the given address and port (-1 == none).
static void expectFound(const std::string& input, unsigned long wantAddr,
                        int wantPort) {
    unsigned long addr = 123;  // poison to catch missing writes
    int port = 999;
    bool ok = extractIPv4(input, addr, port);

    if (ok && addr == wantAddr && port == wantPort) {
        ++g_passed;
    } else {
        ++g_failed;
        std::cout << "FAIL [found]  input=\"" << input << "\"\n"
                  << "      got  ok=" << ok << " addr=" << addr
                  << " port=" << port << "\n"
                  << "      want ok=1 addr=" << wantAddr << " port=" << wantPort
                  << "\n";
    }
}

// Expect no valid address. On failure the contract requires addr=0, port=-1.
static void expectNotFound(const std::string& input) {
    unsigned long addr = 123;
    int port = 999;
    bool ok = extractIPv4(input, addr, port);

    if (!ok && addr == 0 && port == -1) {
        ++g_passed;
    } else {
        ++g_failed;
        std::cout << "FAIL [reject] input=\"" << input << "\"\n"
                  << "      got  ok=" << ok << " addr=" << addr
                  << " port=" << port << "\n"
                  << "      want ok=0 addr=0 port=-1\n";
    }
}

int main() {
    // --- Basic valid addresses -------------------------------------------
    expectFound("1.2.3.4", makeAddr(1, 2, 3, 4), -1);
    expectFound("0.0.0.0", makeAddr(0, 0, 0, 0), -1);
    expectFound("255.255.255.255", makeAddr(255, 255, 255, 255), -1);
    expectFound("192.168.1.1", makeAddr(192, 168, 1, 1), -1);
    expectFound("8.8.8.8", makeAddr(8, 8, 8, 8), -1);

    // --- Valid with port --------------------------------------------------
    expectFound("1.2.3.4:80", makeAddr(1, 2, 3, 4), 80);
    expectFound("0.0.0.0:0", makeAddr(0, 0, 0, 0), 0);
    expectFound("255.255.255.255:65535", makeAddr(255, 255, 255, 255), 65535);
    expectFound("10.0.0.1:8080", makeAddr(10, 0, 0, 1), 8080);
    expectFound("127.0.0.1:1", makeAddr(127, 0, 0, 1), 1);

    // --- Embedded in surrounding garbage ---------------------------------
    expectFound("hello 1.2.3.4 world", makeAddr(1, 2, 3, 4), -1);
    expectFound("addr=192.168.0.1;port", makeAddr(192, 168, 0, 1), -1);
    expectFound("!!!10.20.30.40:500!!!", makeAddr(10, 20, 30, 40), 500);
    expectFound("xx8.8.4.4xx", makeAddr(8, 8, 4, 4), -1);
    expectFound(" \t 172.16.0.1 \t ", makeAddr(172, 16, 0, 1), -1);

    // --- First valid token wins ------------------------------------------
    expectFound("999.999.999.999 1.2.3.4", makeAddr(1, 2, 3, 4), -1);
    expectFound("bad 1.2.3.4 5.6.7.8", makeAddr(1, 2, 3, 4), -1);
    expectFound("1.2.3.4.5 6.7.8.9", makeAddr(6, 7, 8, 9), -1);

    // --- Out-of-range octets ---------------------------------------------
    expectNotFound("256.1.1.1");
    expectNotFound("1.256.1.1");
    expectNotFound("1.1.1.256");
    expectNotFound("300.1.1.1");
    expectNotFound("999.999.999.999");

    // --- Leading zeros (octets) ------------------------------------------
    expectNotFound("01.2.3.4");
    expectNotFound("1.02.3.4");
    expectNotFound("1.2.3.00");
    expectNotFound("00.0.0.0");
    expectNotFound("010.1.1.1");

    // --- Wrong octet count / empty octet ---------------------------------
    expectNotFound("1.2.3");
    expectNotFound("1.2.3.4.5");
    expectNotFound("1.2..4");
    expectNotFound("1...4");
    expectNotFound(".1.2.3.4");
    expectNotFound("1.2.3.4.");
    expectNotFound("1.2.3.4..");

    // --- Too many digits in an octet -------------------------------------
    expectNotFound("1234.1.1.1");
    expectNotFound("1.2.3.4444");

    // --- Colon / port problems -------------------------------------------
    expectNotFound("1.2.3.4:");          // colon but no port
    expectNotFound("1.2.3.4:70000");     // port out of range
    expectNotFound("1.2.3.4:65536");     // port out of range by one
    expectNotFound("1.2.3.4:080");       // leading zero in port
    expectNotFound("1.2.3.4:00");        // leading zero in port
    expectNotFound("1.2.3.4:80:90");     // second colon
    expectNotFound("1.2.3.4::80");       // double colon
    expectNotFound("1.2.3:4");           // colon not after fourth octet
    expectNotFound(":1.2.3.4");          // leading colon glued to token
    expectNotFound("1.2.3.4:123456");    // 6 digit port

    // --- Stray adjacent punctuation makes the whole run invalid ----------
    expectNotFound("1.2.3.4.");
    expectNotFound("1.2.3.4:80.");
    expectNotFound("1.2.3.4:80:");

    // --- Pure garbage / empty --------------------------------------------
    expectNotFound("");
    expectNotFound("no numbers here");
    expectNotFound("....");
    expectNotFound("::::");
    expectNotFound("1234567");

    // --- A rejected glued run does not poison a later valid token ---------
    expectFound("1.2.3.4:80: 9.9.9.9", makeAddr(9, 9, 9, 9), -1);
    expectFound("256.256.256.256 fallback 1.1.1.1:22", makeAddr(1, 1, 1, 1), 22);

    // --- Report ----------------------------------------------------------
    std::cout << "\n----------------------------------------\n";
    std::cout << "Passed: " << g_passed << "\n";
    std::cout << "Failed: " << g_failed << "\n";
    std::cout << "----------------------------------------\n";

    return g_failed == 0 ? 0 : 1;
}
