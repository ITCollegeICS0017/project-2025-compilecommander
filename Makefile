CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Iinclude -MMD -MP          # include path + deps
LDFLAGS ?=
LDLIBS ?=

SRCDIR := src
TESTDIR := tests
BINDIR := bin
TESTBINDIR := $(BINDIR)/tests

APP := $(BINDIR)/exchange_demo

# Sources / objects
SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SRC))
DEPS := $(OBJ:.o=.d)

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
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile objects (+ auto .d dependencies)
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Tests build
tests: $(TEST_BIN)

$(TESTBINDIR)/%: $(TESTDIR)/%.cpp $(OBJ_NO_MAIN)
	@mkdir -p $(TESTBINDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

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

# Debug/ASAN convenience targets (optional)
debug: CXXFLAGS := -std=c++17 -O0 -g3 -Wall -Wextra -Wpedantic
debug: clean all

asan: CXXFLAGS := -std=c++17 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -Wall -Wextra -Wpedantic
asan: LDFLAGS := -fsanitize=address,undefined
asan: clean all

.PHONY: all run tests test clean debug asan

# Include auto-generated dependency files
-include $(DEPS)
