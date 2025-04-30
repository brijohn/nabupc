# Description

This is a ColecoVision ROM loader/emulator for the NABU PC. It will allow loading and running of unmodified ColecoVision ROMs on the NABU. It does this by using a patched version of the ColecoVision BIOS that has the VDP IO address changed to use the NABU address. In addition it has been patched to emulate the SN76489 using the NABU's AY-3-8910 PSG and joysticks.

Some ROMs will need to be patched before use because they do some direct hardware access, the loader has a list of ROMs that need patches and can automatically apply this based on the crc16 of the ROM.


# Usage
The loader uses the HCCA to request paks and consists of three main paks:

 - 000001.nabu - The loader itself
 - 000002.nabu - The customized BIOS
 - 000003.nabu - Patch data

Pak 4 (000004.nabu) is the rom you wish to run.

This system allows the the BIOS or Patch data to be update independently of the loader itself as well as the ROM to be updated easily as well.

 # Build
 In order to build this you will need the following compiler and linker:

  * [vasm](http://sun.hasenbraten.de/vasm/)
  * [vlink](http://sun.hasenbraten.de/vlink/)

To build the loader and associated paks just type ```make```

This will create ```000001.nabu```, ```000002.nabu```, and ```000003.nabu```

# Controller Support
The ColecoVision keypad input is mapped to 0-9,- (*) , =(#) on the NABU keyboard.  Joystick 1 to joystick port 1 and also to the cursor keys with the two triggers mapped to previous page and next page keys. Joystick 2 is mapped to joystick port 2.

# Games
Current games known working:

 - Joust
 - Donkey Kong
 - Donkey Kong Jr.
 - Congo Bongo
 - Smurfs
 - Pitfall
 - Lady Bug
 - Montezuma's Revenge
 - Jumpman Jr.
 - Burger Time
 - Tutankham
