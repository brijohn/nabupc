TYPE      ?= nabu
ASM        = vasmz80_std
LINK       = vlink
ASM_FLAGS  = -Fvobj -Lall
LINK_FLAGS = -brawbin1 -Bforcearchive
OBJS       = loader.o vdp.o menu.o interrupts.o hcca.o emu.o patcher.o


.PHONY: all
all: 000001.nabu 000002.nabu 000003.nabu 000004.nabu

%.o: %.z80
	@echo "Assembling $<..."
	@$(ASM) $(ASM_FLAGS) -L $(patsubst %.z80,%.lst,$<) -o $@ $< >/dev/null

000001.nabu: $(OBJS)
	@echo "Linking loader..."
	@$(LINK) $(LINK_FLAGS) -Tvlink_nabu.cmd -vicelabels coleco.sym -o $@ $^

000002.nabu: bios.o
	@echo "Linking bios..."
	@$(LINK) $(LINK_FLAGS) -Tvlink_bios.cmd -vicelabels bios.sym -o $@ $^

000003.nabu: patches.o
	@echo "Linking patches..."
	@$(LINK) $(LINK_FLAGS) -Tvlink_patches.cmd -vicelabels patches.sym -o $@ $^

000004.nabu: menudata.o
	@echo "Linking menu..."
	@$(LINK) $(LINK_FLAGS) -Tvlink_menu.cmd -vicelabels menudata.sym -o $@ $^

.PHONY: mame
mame: 000001.nabu 000002.nabu 000003.nabu 000004.nabu
	@echo "Creating coleco.npz"
	zip coleco.npz 000001.nabu 000002.nabu 000003.nabu 000004.nabu

.PHONY: clean
clean:
	@rm -f *.o *.npz *.nabu *.sym *.lst
