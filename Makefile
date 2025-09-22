CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

SRCDIR := src
TESTDIR := tests
BINDIR := bin
TESTBINDIR := $(BINDIR)/tests

APP := $(BINDIR)/exchange_demo

# Sources / objects
SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SRC))

# Exclude main.o when linking test binaries
SRC_NO_MAIN := $(filter-out $(SRCDIR)/main.cpp,$(SRC))
OBJ_NO_MAIN := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SRC_NO_MAIN))

# Tests
TEST_SRC := $(wildcard $(TESTDIR)/*.cpp)
TEST_BIN := $(patsubst $(TESTDIR)/%.cpp,$(TESTBINDIR)/%,$(TEST_SRC))

# Default
all: $(APP)

# App link
$(APP): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile objects
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Tests build
tests: $(TEST_BIN)

$(TESTBINDIR)/%: $(TESTDIR)/%.cpp $(OBJ_NO_MAIN)
	@mkdir -p $(TESTBINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run app
run: $(APP)
	./$(APP)

# Run tests (naive runner)
test: tests
	@echo "Running tests…"
	@set -e; for t in $(TEST_BIN); do echo "==> $$t"; $$t; done
	@echo "All tests passed."

# Clean
clean:
	$(RM) -r $(BINDIR)

.PHONY: all run tests test clean
