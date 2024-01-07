

# Arkanoid for the NABU

This is a conversion of the MSX port of Arkanoid to the NABU. It provides an assembler wrapper that will load the original ROM into memory and then patch it to work on the NABU.

This conversion supports both keyboard controls (Arrows + Space) and paddle controls if you are using either MAME or have a modified Atari paddle controller.

## Assemble

 In order to build the rom image, you will need:

   * [z80asm](https://www.nongnu.org/z80asm/)
   * Original ROM image with sha1sum of **2183f07fa3ba87360100b2fa21fda0f55c0f8814**

 Copy the ROM image into the current directory and call it arkanoid.rom

 To build a .nabu file run ```z80asm -o arkanoid.nabu arkanoid.z80```

 To build a .com file edit the line ```CPM: equ 0``` to ```CPM: equ 1``` and run ```z80asm -o arkanoid.com arkanoid.z80```

## Cheats

There are two cheats in this version both are accessed from the title screen.

  * 240 Lives - Hold up and and tap the SYM key 4 times, then press fire to start the game.
  * Continue  - Hold left and and tap the SYM key 4 times, then press fire to start the game.

