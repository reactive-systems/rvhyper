BUILDDIR ?= build
CFG      ?= release
NAME     ?= rvhyper
SRCDIR   ?= src

# Use clang by default.
CC  = clang
CXX = clang++

# Be verbose about the build.
Q ?= @

SRC    := $(sort $(wildcard $(SRCDIR)/*.cpp))

HEADER := $(wildcard $(SRCDIR)/*.h)

BINDIR := $(BUILDDIR)/$(CFG)
BIN    := $(BINDIR)/$(NAME)
OBJ    := $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
DEP    := $(OBJ:%.o=%.d)

# Show compiler warnings (and treat them seriously!)
CXXFLAGS += -Wall -W -Wuninitialized

# Standard compile time flags for C++/CXX projects.
CXXFLAGS += -std=c++11
CXXFLAGS += -I./include

# SPOT lib
LDFLAGS += -lspot -lbddx

-include config/$(CFG).cfg

DUMMY := $(shell mkdir -p $(sort $(dir $(OBJ))))

.PHONY: all clean format demo

all: $(BIN)

-include $(DEP)

clean:
	@echo "===> CLEAN"
	$(Q)rm -fr $(BINDIR)

$(BIN): $(OBJ)
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(BIN) $(OBJ) $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

format:
	@clang-format -style=file -i $(SRC) $(HEADER)
	@clang-format -style=file -i $(SRC) $(HEADER)

demo:
	@./demo.sh
