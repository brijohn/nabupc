

# Choplifter for the NABU

This is a conversion of the MSX port of Choplifter to the NABU. It provides an assembler wrapper that will load the original ROM into memory and then patches it to work on the NABU.

## Assemble

 In order to build the rom image, you will need:

   * [z80asm](https://www.nongnu.org/z80asm/)
   * Original ROM image with sha1sum of **b3ca70c8c8120f8261bb865baf5f57b6506ca02e**

 Copy the ROM image into the current directory and call it choplifter.rom

 To build a .nabu file run ```z80asm -o choplifter.nabu choplifter.z80```

 To build a .com file edit the line ```CPM: equ 0``` to ```CPM: equ 1``` and run ```z80asm -o choplift.com choplifter.z80```

## Controls

Both joystick and keyboard are supported, the keyboard controls use the standard MSX mappings of arrows for movement and space for fire.

In order to change the direction your helicopter is facing hold joystick in the direction you want to face and press fire.

## Cheats

At the title screen press and hold YES then press NO to gain infinite lives when starting the game.
