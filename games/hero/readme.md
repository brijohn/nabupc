

# H.E.R.O for the NABU

This is a conversion of the MSX port of H.E.R.O to the NABU. It provides an assembler wrapper that will load the original ROM into memory and then patch it to work on the NABU.


## Assemble

 In order to build the rom image, you will need:

   * [z80asm](https://www.nongnu.org/z80asm/)
   * Original ROM image with sha1sum of **ebb70722f75279911cce79e6bd78b8f514561b0f**

 Copy the ROM image into the current directory and call it hero.rom

 To build a .nabu file run ```z80asm -o hero.nabu hero.z80```

 To build a .com file edit the line ```CPM: equ 0``` to ```CPM: equ 1``` and run ```z80asm -o hero.com hero.z80```

## Controls

To start a new game press 1-5. This starts at new game at the following levels: 1, 5, 9, 13, 17 (PRO)

The game will play with both joystick and keyboard controls. When playing with the keyboard space is fire and arrows are used for movement.

Pressing Esc will end your game and the PAUSE key with pause your game.
