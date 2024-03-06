# Description

libmsx is a library that helps in converting an MSX ROM to run on the NABU. It contains a partital implementaion of the MSX BIOS and NABU based interrupt handlers needed to get MSX ROMs to run natively on the NABU. You then just need to link in the ROM image along with any custom patches needed by a particular ROM.

# Build

In order to build libmsx you will need the following compiler and linker:

  * [vasm](http://sun.hasenbraten.de/vasm/)
  * [vlink](http://sun.hasenbraten.de/vlink/)

To build the library itself just type ```make libmsx.a```

To build currently supported ROM conversions:
  * Place ROM images you wish to build in root directory
  * Run ```make``` or ```make mame``` to also build npz files to for MAME.

You can also build CP/M com files by running ```make TYPE=cpm```

# Supported ROMs (SHA1)

  * arkanoid.rom - **2183f07fa3ba87360100b2fa21fda0f55c0f8814**
  * bomberman.rom - **5324e053709ff8da6c18ae4afba6a2e0c3a722ba**
  * choplifter.rom - **b3ca70c8c8120f8261bb865baf5f57b6506ca02e**
  * hyperraly.rom - **7dfe091e02f6c1b21bd65e44eb5052d564d0ef92**
  * thexder.rom - **a2109da08b1921c4b3b47c6847598d424581b508**
  * roadfighter.rom - **bf4a0bab1e8eaa70a8b12cd9a81d7a90a74c9e26**


## Arkanoid

### Controls

Both keyboard controls (Arrows + Space) and paddle controls are supported.

Paddles are supported under MAME, however if you are running on metal you will need to [modify](https://www.nabunetwork.com/download-manager.php?id=64) a set of Atari paddles to work with thw NABU.

### Cheats

There are two cheats in this version both are accessed from the title screen.

  * 240 Lives - Hold up and and tap the SYM key 4 times, then press fire to start the game.
  * Continue  - Hold left and and tap the SYM key 4 times, then press fire to start the game.

## Bomberman

### Controls

Both joystick and keyboard are supported, the keyboard controls use the standard MSX mappings of arrows for movement and space for fire.

## Choplifter

### Controls

Both joystick and keyboard are supported, the keyboard controls use the standard MSX mappings of arrows for movement and space for fire.

In order to change the direction your helicopter is facing hold joystick in the direction you want to face and press fire.

### Cheats

At the title screen press and hold YES then press NO to gain infinite lives when starting the game.

## Hyper Rally

### Controls

Both joystick and keyboard controls are supported. Steering is done with the left/right arrows, while pressing up will engage your breaks. Pressing space/fire will accelerate.

You can shift into high gear for overtaking other cars by coming up to speed and then releasing throttle and quickly pressing it again. Switching back to low gear will happen automatically if you reduce your speed.

## THEXDER

### Controls

Keyboard controls use the arrows for movement, space is fire, and SYM activates shields.

Joystick controls are also supported however you can't activate shields via the joystick as the NABU only has one button joysticks.

## Roadfighter

### Controls

Keyboard controls use the left/right arrows for movement, and space for throttle. Joysticks are also supported. Releasing the throttle will cayse your car to slow down.
