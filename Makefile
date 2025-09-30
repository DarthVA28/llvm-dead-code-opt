SHELL := /bin/bash
BUILD := build
LLVM_DIR := $(shell llvm-config-18 --cmakedir)
ZSTD_CMAKE := /usr/local/lib/cmake/zstd  

PLUGIN := libDCEPass.so

all: $(BUILD)/$(PLUGIN)

configure:
	cmake -S . -B $(BUILD) -G Ninja \
	  -DLLVM_DIR="$(LLVM_DIR)" \
	  -DCMAKE_PREFIX_PATH="$(ZSTD_CMAKE)"

build: configure
	cmake --build $(BUILD) --parallel

$(BUILD)/$(PLUGIN): build

clean:
	-rm -rf $(BUILD)
