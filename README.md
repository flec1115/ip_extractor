# IP Extractor

A small C++ program that scans a line of text and extracts the first valid
IPv4 address (with an optional `:port` suffix) embedded anywhere in it.

## Requirements

- A C++17 compiler (`g++` by default)
- `make`

## Project layout

| File            | Purpose                                              |
| --------------- | ---------------------------------------------------- |
| `extractor.h`   | Public declaration of `extractIPv4`.                 |
| `extractor.cpp` | The extraction/validation logic.                     |
| `main.cpp`      | Interactive command-line front end.                  |
| `tests.cpp`     | Self-contained test suite (no external framework).   |
| `Makefile`      | Build and test targets.                              |

## Building

Build the main program:

```sh
make
```

This produces the `ip_extractor` executable.

## Running

Start the program and type (or paste) text at the prompt. The program prints
the extracted address, its 32-bit decimal value, and the port (or `none`).
Type `END` or press `Ctrl-D` to quit.

```sh
./ip_extractor
```

Example session:

```
Enter text (or END to quit): connect to 192.168.1.1:8080 now
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: 8080)
Enter text (or END to quit): no address here
No valid IPv4 address found.
Enter text (or END to quit): END
Program terminated.
```

You can also pipe input in:

```sh
echo "server at 8.8.8.8" | ./ip_extractor
```

## Testing

Build and run the test suite in one step:

```sh
make test
```

This compiles `tests.cpp` into the `run_tests` executable and runs it. The
harness prints a summary and exits with status `0` when all tests pass, or `1`
if any fail:

```
----------------------------------------
Passed: 57
Failed: 0
----------------------------------------
```

To build the test binary without running it:

```sh
make run_tests
./run_tests
```

## Cleaning

Remove all build artifacts (executables and object files):

```sh
make clean
```
