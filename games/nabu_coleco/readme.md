# Description

This is a ColecoVision ROM loader/emulator for the NABU PC. It will allow loading and running of unmodified ColecoVision ROMs on the NABU. It does this by using a patched version of the ColecoVision BIOS that has the VDP IO address changed to use the NABU address. In addition it has been patched to emulate the SN76489 using the NABU's AY-3-8910 PSG and joysticks.

Some ROMs will need to be patched before use because they do some direct hardware access, the loader has a list of ROMs that need patches and can automatically apply this based on the crc16 of the ROM.


# Usage
The loader uses the HCCA to request paks and consists of four main paks:

 - 000001.nabu - The loader itself
 - 000002.nabu - The customized BIOS
 - 000003.nabu - Patch data
 - 000004.nabu - Menu data

Paks 5+ are the ColecoVision ROMS

This system allows the the BIOS or Patch/Menu data to be update independently of the loader itself as well as the ROM to be updated easily as well.

 # Build
 In order to build this you will need the following compiler and linker:

  * [vasm](http://sun.hasenbraten.de/vasm/)
  * [vlink](http://sun.hasenbraten.de/vlink/)

To build the loader and associated paks just type ```make```

This will create ```000001.nabu```, ```000002.nabu```, ```000003.nabu```, and ```000004.nabu```

# Controller Support
The ColecoVision keypad input is mapped to 0-9,- (*) , =(#) on the NABU keyboard.  The joystick is mapped onto the cursor keys with the two triggers mapped to previous page and next page keys. You can switch which ColecoVision controller the joystick and keypad are mapped to by using the SYM key on the nabu keyboard.

The NABU joystick is also mapped to the currently selected ColecoVision controller, but has a few limitations. The first is that it only supports a single trigger, the second one is that only a single joystick is supported (though the SYMN does switch which ColecoVision controller it operates). Both of these are due to an issue with handling multi byte scan codes while running a ColecoVision game.

# Games
Current games known working:

 - Burger Time
 - Carnival
 - Congo Bongo
 - Donkey Kong
 - Donkey Kong Jr.
 - Joust
 - Jumpman Jr.
 - Lady Bug
 - Montezuma's Revenge
 - Mr Do.
 - Pepper II
 - Pitfall
 - Pitfall II
 - Q-Bert II
 - Smurfs
 - Tutankham
