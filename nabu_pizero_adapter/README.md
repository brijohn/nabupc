# Description
This is a carrier board designed for a Raspberry Pi Zero W. It was made to fit in the empty case of an nabu network cable adapter and host a NABU Network Emulator allowing your nabu to request and load software.

The uses a MAX3070 to produce a RS422 signal from the PI's TTL UART and has a super capacitor based UPS to allow the pi time to cleanly shutdown in the event of a power loss. It has has a header that the front panel LEDS can be connected to and then controlled by the PI.

# Setup
In addition to this board you will also need a Raspberry PI Zero W. Though a suitable alternative is the bananapi M2 Zero, which has the same pinout and form factor, it also includes a U.FL connector for an external antenna, whereas the raspberry pi needs to modified.

The external antenna can be mounted in the existing F type connector hole in the back of the case, though since an SMA connector is smaller then an F type you will need a couple of washers on each side of the antenna's SMA connector to mount it.

The code used with the ATTiny85 to monitor the power status and report it to the PI over I2C was written by Scott M. Baker and can be found at https://github.com/sbelectronics/ups/.

# BOM

|         Item        |     Reference     | Quantity |             Digikey              |
|---------------------|-------------------|----------|----------------------------------|
|  0.1uf Capacitor    |  C3, C4, C5, C6   |     4    |          490-9197-1-ND           |
| 50F Super-capacitor |      C1, C2       |     2    |          1182-1051-ND            |
|    1N5820 Diode     |      D1, D3       |     2    |      112-1N5820-E3/73CT-ND       |
|   5.6V Zener Diode  |        D2         |     1    |         1N4734AFSCT-ND           |
|   5.1V Zener Diode  |        D4         |     1    |         1N4733AFSCT-ND           |
|      1A Fuse        |        F1         |     1    |             F2313-ND             |
|  Single Row Headers |    J3, J4, J5     |     1    |          S1012EC-40-ND           |
|      5-pin DIN      |        J2         |     1    |            CP-2350-ND            |
|    180 Resistor     |        R1         |     1    |     13-MFR-25FTE52-180RCT-ND     |
|    220 Resistor     | R9, R10, R11, R12 |     4    |             220QBK-ND            |
|    1k Resistor      |      R3, R4       |     2    |             1.0KQBK-ND           |
|   7.5k Resistor     |      R6, R8       |     2    |             7.5KQBK-ND           |
|   10k Resistor      |      R5, R7       |     2    |      13-CFR-25JT-52-10KCT-ND     |
|   2.2 3W Resistor   |        R2         |     1    |            A138434CT-ND          |
|      ATtiny85V      |        U3         |     1    |         ATTINY85V-10PU-ND        |
|       MAX3070       |        U1         |     1    |          MAX3070EEPD+-ND         |
|   Shunt Regulator   |      U5, U6       |     2    |            296-50325-ND          |
|       ULN2803A      |        U2         |     1    |            497-2356-5-ND         |
|      5V Step-Up     |        U4         |     1    | 2183-2891-ND, S7037-ND, S7038-ND |
