; ------------------------------------------
;  NABU I/O ports
; ------------------------------------------
.set          VDP_LATCH, 0xa1
.set          VDP_DATA,  0xa0

.set          PSG_LATCH, 0x41
.set          PSG_WRITE, 0x40
.set          PSG_READ,  0x40

.set          KBD_DATA,  0x90
.set          KBD_STAT,  0x91


; ------------------------------------------
;  MSX Memory Locations
; ------------------------------------------
.set          T32NAM, 0xf3bd
.set          T32COL, 0xf3bf
.set          T32CGP, 0xf3c1
.set          T32ATR, 0xf3c3
.set          T32PAT, 0xf3c5

.set          GRPNAM, 0xf3c7
.set          GRPCOL, 0xf3c9
.set          GRPCGP, 0xf3cb
.set          GRPATR, 0xf3cd
.set          GRPPAT, 0xf3cf

.set          MLTNAM, 0xf3d1
.set          MLTCOL, 0xf3d3
.set          MLTCGP, 0xf3d5
.set          MLTATR, 0xf3d7
.set          MLTPAT, 0xf3d9

.set          RG0SAV, 0xf3df
.set          RG1SAV, 0xf3e0
.set          FORCLR, 0xf3e9
.set          BAKCLR, 0xf3ea
.set          BDRCLR, 0xf3eb

.set          PUTPNT, 0xf3f8
.set          GETPNT, 0xf3fa

.set          NAMBAS, 0xf922
.set          CGPBAS, 0xf924
.set          PATBAS, 0xf926
.set          ATRBAS, 0xf928

.set          KEYBUF, 0xfbf0

.set          SCRMOD, 0xfcaf

.set          H_KEYI, 0xfd9a
.set          H_TIMI, 0xfd9f
