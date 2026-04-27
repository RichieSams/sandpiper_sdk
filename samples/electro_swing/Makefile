# Check OS type
ifeq ($(OS),Windows_NT)
	UNAME := Windows
else
	UNAME := $(shell uname -a)
	ifeq ($(findstring xilinx,$(UNAME)),)
		UNAME := Linux
	else
		UNAME := Petalinux
	endif
endif

TARGET = electro_swing
DEBUG ?= 0

default: $(TARGET)

# Directories
src_dir = .
bin_dir = bin
sdk_dir = ../../SDK

# Rules
ifeq ($(UNAME), Windows)
# Windows
ARM_GCC ?= arm-none-linux-gnueabihf-g++
ARM_CC  ?= arm-none-linux-gnueabihf-gcc
else ifeq ($(UNAME), Linux)
# Linux
ARM_GCC ?= arm-linux-gnueabihf-g++
ARM_CC  ?= arm-linux-gnueabihf-gcc
else
# Petalinux
ARM_GCC ?= arm-amd-linux-gnueabi-g++
ARM_CC  ?= arm-amd-linux-gnueabi-gcc
endif

ARCH_OPTS    := -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard

ifeq ($(DEBUG),1)
OPT_OPTS     := -O0 -g3 -fno-omit-frame-pointer
else
OPT_OPTS     := -Ofast -flto
endif

ARM_GCC_OPTS += -std=c++20 $(OPT_OPTS) $(ARCH_OPTS)
ARM_CC_OPTS  += $(OPT_OPTS) $(ARCH_OPTS)
ARM_GCC_LIBS += -lgcc -lc -lm
DEPFLAGS     := -MMD -MP

incs += -I$(src_dir) -I$(sdk_dir)/include

# Source files
cpp_srcs := $(wildcard $(src_dir)/*.cpp)
c_srcs   := $(wildcard $(src_dir)/*.c)

# Object files in bin directory
cpp_objs := $(patsubst $(src_dir)/%.cpp,$(bin_dir)/%.o,$(cpp_srcs))
c_objs   := $(patsubst $(src_dir)/%.c,$(bin_dir)/%.o,$(c_srcs))

all_objs := $(cpp_objs) $(c_objs)
all_deps := $(all_objs:.o=.d)

-include $(all_deps)

# Create bin directory
$(bin_dir):
	@mkdir $(bin_dir)

# Build SDK library if needed
$(sdk_dir)/libsandpiper.a:
	$(MAKE) -C $(sdk_dir) DEBUG=$(DEBUG)

# Compile local .cpp files
$(bin_dir)/%.o: $(src_dir)/%.cpp | $(bin_dir)
	$(ARM_GCC) $(ARM_GCC_OPTS) $(DEPFLAGS) $(incs) -c $< -o $@

# Compile local .c files
$(bin_dir)/%.o: $(src_dir)/%.c | $(bin_dir)
	$(ARM_CC) $(ARM_CC_OPTS) $(DEPFLAGS) $(incs) -c $< -o $@

# Link target
$(TARGET): $(sdk_dir)/libsandpiper.a $(all_objs)
	$(ARM_GCC) $(ARM_GCC_OPTS) -o $@ $(all_objs) -L$(sdk_dir) -lsandpiper $(ARM_GCC_LIBS)

.PHONY: debug
debug:
	$(MAKE) DEBUG=1

.PHONY: release
release:
	$(MAKE) DEBUG=0

.PHONY: generate_assets
generate_assets:
	uv run ./generate_assets.py

.PHONY: generate_maps
generate_maps:
	uv run ./generate_maps.py

.PHONY: clean
clean:
ifeq ($(UNAME), Windows)
	@-del /F /Q $(TARGET)
	@-rmdir /S /Q $(bin_dir)
	@-if exist "$(sdk_dir)\\libsandpiper.a" del /F /Q "$(sdk_dir)\\libsandpiper.a"
else
	@-rm -f $(TARGET)
	@-rm -rf $(bin_dir)
	@-rm -f $(sdk_dir)/libsandpiper.a
endif
