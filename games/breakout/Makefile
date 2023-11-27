export Z88DK_DIR := /home/bjohnson/Software/z88dk/
export ZCCCFG := ${Z88DK_DIR}/share/z88dk/lib/config/
export PATH := ${Z88DK_DIR}bin:${PATH}
export NABU_LIB := $(shell git rev-parse --show-toplevel)/../NABU-LIB/NABULIB

TARGET=000001.nabu
NPZ=breakout

all: ${TARGET}

${TARGET}: ${NPZ}.c
	zcc +nabu -vn --list -m -compiler=sdcc -I${NABU_LIB} -o $@ $^

.PHONY: mame
mame: ${TARGET}
	zip ${NPZ}.npz ${TARGET}

.PHONY: clean
clean:
	@rm -rf *.npz 000001.* *.lis
