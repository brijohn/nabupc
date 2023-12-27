

# River Raid for the NABU

This is a conversion of the MSX port of RiverRaid to the NABU. It provides an assembler wrapper that will load the original ROM into memory and then patch it to work on the NABU.


## Assemble

 In order to build the rom image, you will need:

   * [z80asm](https://www.nongnu.org/z80asm/)
   * Original ROM image with sha1sum of **a1e14912d45944b9a6baef1d4d3a04c1ae8df923**

 Copy the ROM image into the current directory and call it RiverRaid.rom

 To build a .nabu file run ```z80asm -o riverraid.nabu riverraid.z80```

## Known issues

 This runs fine under MAME emulation, however when run on metal there are graphical artifacts that appear along the shorelines. It is still playable though. I have not yet been able to figure out what the cause is.
