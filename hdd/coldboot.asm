; Small HDD cold boot loader, will load track 0, sectors 1-15 to address 0x100 and then begin executation.
; Sectors are 0 based, so sector 0 is the first sector, sector 1is the second sector, etc
; Author: Brian Johnson (c) 2022

DATAREG        equ   $00
ERRORREG       equ   $01
SECTORCREG     equ   $02
SECTORNREG     equ   $03
CYLLOREG       equ   $04
CYLHIREG       equ   $05
SDHREG         equ   $06
STATUSREG      equ   $07
CMDREG         equ   $07

	ORG $C000                      ; Cold Boot Loader is loaded at $C000 by the ROM

	ld ix, ioport                  ; IO Port array used by HDD
	ld a, $cf                      ; Controller Status Port for Slot 1
	ld b, 4
scan_hdd:                          ; Scan each slot to find the slot with the HDD card
	ld c, a                        ; Store the status reg for later
	in a, (c)                      ; Read from current status register
	cp $e8                         ; If $E8 is returned we are a HDD controller
	jr z, found_hdd
	ld a, c
	add a, $10                     ; Add 16 to status register
	djnz scan_hdd                  ; and try the next slot.
	halt                           ; No HDD controller halt system
found_hdd:
	ld a,c
	and $f0                        ; Keep the base base register for accessing the drive we founde
	ld (ioport + DATAREG), a       ; Store the Data regsier
	inc a
	ld (ioport + ERRORREG), a      ; Store the error/write precomp register
	inc a
	ld (ioport + SECTORCREG), a    ; Sector count register
	inc a
	ld (ioport + SECTORNREG), a    ; Sector number register
	inc a
	ld (ioport + CYLLOREG), a      ; Cylinder (low) register
	inc a
	ld (ioport + CYLHIREG), a      ; Cylinder (high) register
	inc a
	ld (ioport + SDHREG), a        ; SDH register
	inc a
	ld (ioport + STATUSREG), a     ; Status / Command register

	ld hl, $100                    ; Address to load cpmldr at
	ld a, $01                      ; Start reading from the second sectorr
	ld (sector), a
	ld b, 15                       ; Read the next 16 sectors
rdtr:
	call read_sector               ; Read sector
	ld a, (sector)                 ;
	inc a                          ; Increment sector
	ld (sector), a                 ;
	djnz rdtr                      ; Next sector
	jp $100                        ; Jump to cpmldr

; HL = Load Adress
read_sector:
	push bc                        ; Save BC
	ld a, $01
	ld c, (ix + SECTORCREG)
	out (c), a                     ; Load sector count of 1 into register

	ld a, (sector)                 ; Sector number to read
	ld c, (ix + SECTORNREG)
	out (c), a                     ; Load sector into register

	xor a                          ; CPMLDR only uses the first cylinder, and head so them them all to zero
	ld c, (ix + CYLLOREG)
	out (c), a

	ld c, (ix + CYLHIREG)
	out (c), a

	or a, $20                      ; SDH = no CRC, 512 byte sectors, drive 1, head 0
	ld c, (ix + SDHREG)
	out (c), a

	ld a, $75                      ; $75 = seek command
	ld c, (ix + CMDREG)
	out (c), a                     ; Seek to CHS value
	call wait_nbusy                ; Wait till drive is no longer busy

	ld a, $20                      ; $20 = read (single sector) command
	ld c, (ix + CMDREG)
	out (c), a                     ; Perform read

	call wait_nbusy                ; Wait till drive is no longer busy
	ld b, 00                       ; This will repat the inir command 256 times
	ld c, (ix+DATAREG)             ; HDD Data port

	inir                           ; Read 256 bytes from disk
	inir                           ; Read the next 256 bytes
	pop bc                         ; Restore BC
	ret

wait_nbusy:
	ld c, (ix + STATUSREG)         ; Status Port
	in a, (c)                      ; Read Status Port
	bit 7, a                       ; Check bit 7 (BSY bit)
	jr nz, wait_nbusy              ; While bit 7 is set
	ret

ioport:
	db 00, 00, 00, 00, 00, 00, 00, 00

sector:
	db 00


