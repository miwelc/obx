LIBDIR = .
INCLUDEDIR = .
SRCDIR = impl
OBJDIR = obj

DEBUG = 0

ifeq ($(DEBUG), 1)
	DEBUG_FLAGS = -g3 -O1
else
	DEBUG_FLAGS = -static -static-libstdc++ -static-libgcc -O3 -flto -ffat-lto-objects
endif

CXX = g++
AR = gcc-ar
COMMON_FLAGS = -Werror -Wall -pedantic -Wno-unused-function $(DEBUG_FLAGS) -fdiagnostics-color=auto
CPPFLAGS = $(COMMON_FLAGS) -I$(INCLUDEDIR) -std=c++17

uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
OBJS += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(shell find $(SRCDIR) -name '*.cpp'))
OBJDIRS = $(call uniq,$(dir $(OBJS)))
OBJDIRS += $(LIBDIR)

$(LIBDIR)/obx.a: $(OBJS)
	$(AR) crvs $@ $(OBJS)
	@echo "Compiled successfully"

$(OBJS): | $(OBJDIRS)

$(OBJDIRS):
	mkdir -p $@

$(OBJDIR)/%.o:	$(SRCDIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@ -include obx.h

clean:
	rm -rf $(OBJDIR)/* $(LIBDIR)/obx.a
