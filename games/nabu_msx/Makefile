
ASM        = vasmz80_std
LINK       = vlink
ASM_FLAGS  = -Fvobj -DMSX_LOAD=0x4000
LINK_FLAGS = -brawbin1 -Tvlink_nabu.cmd
OBJS       = loader.o splash.o patcher.o interrupts.o cbios.o font.o
PORTS      = $(basename $(wildcard *.rom))
TARGETS    = $(patsubst %,%.nabu,$(PORTS))

.PHONY: all
all: $(TARGETS)

%.o: %.z80
	@echo "Assembling $<..."
	@$(ASM) $(ASM_FLAGS) -o $@ $< >/dev/null

libmsx.a: $(OBJS)
	ar cr $@ $^

define MSX_PORT
$(1).o: $(1)/$(1).z80
	@echo "Assembling $$<..."
	@$(ASM) $(ASM_FLAGS) -o $$@ $$< >/dev/null

$(1).nabu: $(OBJS) $(1).o
	@echo "Linking $(1).nabu..."
	@$(LINK) $(LINK_FLAGS) -o $$@ $$^
endef

$(foreach port,$(PORTS),$(eval $(call MSX_PORT,$(port))))

.PHONY: mame
mame: $(TARGETS)
	@for target in $^; do \
		echo "Creating $${target%.*}.npz"; \
		cp $${target%.*}.nabu 000001.nabu; \
		zip $${target%.*}.npz 000001.nabu; \
	done
	@rm -f 000001.nabu

.PHONY: clean
clean:
	@rm -f *.o *.a *.npz $(TARGETS)
