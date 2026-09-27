# AI Usage Disclosure

> Template — fill in each section below to document how (or whether) AI tools
> were used in producing this assignment.

## Assignment

- **Course:** EECS 581
- **Assignment:** IPv4 Address Extractor
- **Name: Felix Balandran**
- **Date: 09-27-2026**

## Did you use AI tools on this assignment?

- [x] Yes
- [ ] No

## Tools Used

<!-- e.g. ChatGPT, Claude, GitHub Copilot, Gemini, etc. Include version/model if known. -->

| Tool | Version / Model | Date Consulted |
| ---- | --------------- | -------------- |
|Claude    |Opus 4.8             |09-27-2026                |

## What the AI Was Used For

<!-- Describe each way AI was used: brainstorming, generating code, writing
     tests, debugging, explaining concepts, writing documentation, etc. Be
     specific about which files or parts were affected. -->

- AI was used for generating the extractor, main, test, and makefile 
- AI also generated the test cases initially
- Used AI to create the md template to display the way AI was used

## Prompts Used


```
Problem description

  Write a C or C++ program that reads a line of text and extracts a single valid IPv4 address — optionally followed by a port number — embedded anywhere in that text. Only digits, periods (.), and colons (:) are ever part of a valid token; every other character is garbage and is skipped. A candidate token must match the address grammar in full — no partial matches, no truncating to find a valid piece inside a longer run.

  An address is four octets separated by periods (octet.octet.octet.octet), each octet 1–3 digits, value 0–255, no leading zero unless the value is exactly 0. An optional :port may follow the fourth octet: 1–5 digits, value 0–65535, same leading-zero rule. If a colon is present, the port must be fully valid or the entire match — address included — is rejected.
  Function prototype

  The function returns whether a valid address was found, and delivers the address and port through two additional parameters.

  C:
  // Returns 1 if a valid address was found, 0 otherwise.
  // On success: *outAddress holds the 32-bit value, and
  // *outPort holds the port number, or -1 if no port was present.
  // On failure: *outAddress is set to 0 and *outPort is set to -1.
  int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);

  C++

  // Returns true if a valid address was found, false otherwise.
  // On success: outAddress holds the 32-bit value,
  // and outPort holds the port number, or -1 if no port was present.
  // On failure: outAddress is set to 0 and outPort is set to -1.
  bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);
  Built-in functions/libraries you may not use

      Any string-to-number conversion function: atoi, atol, atoll, strtol, strtoul, strtod, stoi, stol, stoul, sscanf, scanf with numeric conversions.
      Any address-parsing library function: inet_aton, inet_pton, inet_addr, or equivalents.
      Any regular-expression facility (std::regex, POSIX regex.h, or similar) — the parsing and validation logic must be your own character-by-character code, not a pattern matched by a library.
      Standard character-classification functions (isdigit, etc.) are fine to use.

  Other requirements

      Exactly one valid address may be extracted per input line; everything else in the line is either garbage (skipped) or part of a candidate token that fails validation.
      Reject anything that does not exactly match the grammar above — wrong octet count, empty octet, out-of-range octet or port, a disallowed leading zero, a second colon, a colon not immediately after the fourth octet, or a stray period/colon directly adjacent to an otherwise-valid address.
      On success, print: Extracted IPv4 address: A.B.C.D (decimal value: N, port: P) where N is the 32-bit decimal value and P is the port number or the literal text none.

  Program requirements

      Input loop: main continuously prompts the user for input until the user enters END (case-sensitive), then prints Program terminated. and exits.
      Extraction function: implement extractIPv4 exactly as prototyped above. All digit accumulation must be done by hand.
      Display: main receives the result from extractIPv4 and formats the output exactly as specified above.

  . Create the program in c++ based on the previous instructions and requirements. Along with the main IP extractor, create a set of test cases with a make file. Finally create a tempalte md file where my AI usage discolusure will be inputed

```

## AI-Generated Content and Your Modifications

<!-- Describe what the AI produced and how you reviewed, edited, tested, or
     rewrote it. Which parts are AI-authored vs. human-authored? -->

- 

## Verification


- Reviewed the all of the produced files. Made sure to more carefully review the tests.cpp and Makefile.
- Edited the test inputs myself to make sure AI did not create biased test cases.
- 

## Attestation

I affirm that the above disclosure is accurate and complete, and that I
understand and can explain all code submitted regardless of how it was produced.

- **Signature:**
- **Date:**
