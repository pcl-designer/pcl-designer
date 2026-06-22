# ==========================================================================
# Portable Makefile for DesignWizardVn_App_GapPrimary.
#
# Build:        make
# Debug build:  make debug
# Clean:        make clean
#
# Requires OpenMP support.
#   - Linux (gcc/clang): -fopenmp is built in
#   - macOS (Apple clang): requires Homebrew libomp
#                          install with:  brew install libomp
#   - Windows (MinGW):    -fopenmp is built in
# ==========================================================================

TARGET  := DesignWizardVn_App_GapPrimary

CC      ?= cc
CFLAGS  ?= -O2 -fwrapv -ffp-contract=off
LDLIBS  ?= -lm

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS Apple clang needs explicit libomp paths from Homebrew.
    # Try three detection paths in order so the build works whether or not
    # `brew` is on PATH at make-time (MATLAB's system() inherits a sanitized
    # PATH that often omits /opt/homebrew/bin and /usr/local/bin):
    #   1. `brew --prefix libomp`   - works in interactive terminals
    #   2. /opt/homebrew/opt/libomp - Apple Silicon Homebrew standard path
    #   3. /usr/local/opt/libomp    - Intel Homebrew standard path
    BREW_LIBOMP := $(shell brew --prefix libomp 2>/dev/null)
    ifeq ($(BREW_LIBOMP),)
        ifneq ($(wildcard /opt/homebrew/opt/libomp/include/omp.h),)
            BREW_LIBOMP := /opt/homebrew/opt/libomp
        else ifneq ($(wildcard /usr/local/opt/libomp/include/omp.h),)
            BREW_LIBOMP := /usr/local/opt/libomp
        endif
    endif
    ifeq ($(BREW_LIBOMP),)
        $(error libomp not found. Install via: brew install libomp. If already installed and you see this error from MATLAB, confirm omp.h exists at /opt/homebrew/opt/libomp/include/ (Apple Silicon) or /usr/local/opt/libomp/include/ (Intel))
    endif
    OMP_CFLAGS := -Xpreprocessor -fopenmp -I$(BREW_LIBOMP)/include
    OMP_LDLIBS := -L$(BREW_LIBOMP)/lib -lomp
else
    # Linux / MinGW
    OMP_CFLAGS := -fopenmp
    OMP_LDLIBS := -fopenmp
endif

override CFLAGS += -I. $(OMP_CFLAGS)
override LDLIBS += $(OMP_LDLIBS)

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

debug: CFLAGS := -O0 -g -fwrapv -fsanitize=address
debug: LDFLAGS := -fsanitize=address
debug: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
