# Makefile for the IPv4 extractor program and its test suite.

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2

# Object files shared between the program and the tests.
COMMON_OBJ := extractor.o

APP        := ip_extractor
TEST_APP   := run_tests

.PHONY: all test clean

# Default target: build the main program.
all: $(APP)

# Main program.
$(APP): main.o $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Test executable.
$(TEST_APP): tests.o $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build and run the test suite.
test: $(TEST_APP)
	./$(TEST_APP)

# Pattern rule for object files. Rebuild when the header changes.
%.o: %.cpp extractor.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(APP) $(TEST_APP) *.o
