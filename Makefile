BINDIR = bin
INCLUDEDIR = include
SRCDIR = src
OBJDIR = obj

DEBUG = 0

ifeq ($(DEBUG), 1)
	DEBUG_FLAGS = -g3 -O1
else
	DEBUG_FLAGS = -O3 -flto -fno-fat-lto-objects
endif

CXX = g++
COMMON_FLAGS = -Werror -Wall -pedantic -Wno-unknown-pragmas $(DEBUG_FLAGS) -fdiagnostics-color=auto
CPPFLAGS = $(COMMON_FLAGS) -I$(INCLUDEDIR) -std=c++17
LD = $(CXX)
LDFLAGS = $(COMMON_FLAGS)

uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
OBJS += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(shell find $(SRCDIR) -name '*.cpp'))
OBJDIRS = $(call uniq,$(dir $(OBJS)))
OBJDIRS += $(BINDIR)

$(BINDIR)/test:	$(OBJS)
	$(LD) -o $@ $(filter-out $(wildcard */**/*.a),$(OBJS)) $(LDFLAGS)
	@echo "Compiled successfully"

$(OBJS): | $(OBJDIRS)

$(OBJDIRS):
	mkdir -p $@

$(OBJDIR)/%.o:	$(SRCDIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/test
