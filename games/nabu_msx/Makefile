TYPE      ?= nabu
ASM        = vasmz80_std
LINK       = vlink
ASM_FLAGS  = -Fvobj -DMSX_LOAD=0x4000
LINK_FLAGS = -brawbin1 -Tvlink_${TYPE}.cmd -Bforcearchive
OBJS       = loader.o splash.o patcher.o interrupts.o cbios.o font.o
PORTS      = $(basename $(wildcard *.rom))

ifeq ($(TYPE), nabu)
EXT    = nabu
else ifeq ($(TYPE), cpm)
EXT    = com
else
$(error TYPE must be either nabu or cpm)
endif

TARGETS    = $(patsubst %,%.$(EXT),$(PORTS))

.PHONY: all
all: $(TARGETS)

%.o: %.z80
	@echo "Assembling $<..."
	@$(ASM) $(ASM_FLAGS) -o $@ $< >/dev/null

libmsx.a: $(OBJS)
	@echo "Creating libmsx.a..."
	@ar cr $@ $^

define MSX_PORT
$(1).o: $(1)/$(1).z80
	@echo "Assembling $$<..."
	@$(ASM) $(ASM_FLAGS) -o $$@ $$< >/dev/null

$(1).$(EXT): libmsx.a $(1).o
	@echo "Linking $(1).$(EXT)..."
	@$(LINK) $(LINK_FLAGS) -vicelabels $(1).lst -o $$@ $$^
endef

$(foreach port,$(PORTS),$(eval $(call MSX_PORT,$(port))))

.PHONY: type_check
type_check:
ifeq ($(TYPE), cpm)
	$(error Only supported when TYPE is nabu)
endif

.PHONY: mame
mame: type_check $(TARGETS)
	@for target in $(TARGETS); do \
		echo "Creating $${target%.*}.npz"; \
		cp $${target%.*}.nabu 000001.nabu; \
		zip $${target%.*}.npz 000001.nabu; \
	done
	@rm -f 000001.nabu

.PHONY: clean
clean:
	@rm -f *.o *.a *.npz *.com *.nabu *.lst
