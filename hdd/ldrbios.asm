; LDRBIOS for NABU PCs to allow booting from the HDD
; Compile to REL format and link with the CPMLDR.REL to create CPMLDR.COM
; Sectors are 0 based, so sector 0 is the first sector, sector 1 is the second sector, etc
; Author: Brian Johnson (c) 2022

DATAREG    equ $00
ERRREG     equ $01
WRITEPCREG equ $02
SECTORCREG equ $03
SECTORNREG equ $04
CYLLOREG   equ $05
CYLHIREG   equ $06
SDHREG     equ $07
STATUSREG  equ $08
CMDREG     equ $09
DRIVEREG   equ $0a

TRACKLO    equ $0b
TRACKHI    equ $0c
SECTORLO   equ $0d
SECTORHI   equ $0e
DMALO      equ $0f
DMAHI      equ $10

	cseg

;  BIOS Vector
boot:      jp bootf
wboot:     jp stub1

const:     jp stub1
conin:     jp stub1
conout:    jp stub1
list:      jp stub1
punch:     jp stub1
reader:    jp stub1

home:      jp homef
seldsk:    jp seldskf
settrk:    jp settrkf
setsec:    jp setsecf
setdma:    jp setdmaf
read:      jp readf
write:     jp stub1

listst:    jp stub1
sectran:   jp sectranf

conost:    jp stub1
auxist:    jp stub1
auxost:    jp stub1
devtbl:    jp stub1
devini:    jp stub1
drvtbl:    jp stub1
multiio:   jp stub1
flush:     jp stub1
move:      jp movef

bootf:
	di                             ; Turn of interrupts, CPM will enable them when its ready
	ld a, $c3
	ld ($0000), a
	ld ($0005), a
	ld hl, $b03
	ld ($0001), hl
	ld hl, ($2e62)
	ld ($0006), hl
	call initialize_disk          ; Initialize IO Port array and recalibrate (seek 0) HDD
	ret

homef:
	xor a
	ld (track), a                  ; Set track to 0...
	ld (sector), a                 ; and sector too
	ret

seldskf:
	ld hl, dph                     ; Return Fixed DPH configured for a 10 MB NABU HDD
	ret

settrkf:
	ld (track), bc                 ; Store track value
	ret

setsecf:
	ld (sector), bc                ; Store sector value
	ret

setdmaf:
	ld (dma), bc                   ; store DMA address
	ret

readf:
	call drive_seek                ; Seek to selected track and sector values
	jr nz, read_failed             ; Did we succeed?
	call drive_read                ; Ok read this sector into the current DMA address.
	jr nz, read_failed             ; Did the read succeed?
	xor a                          ; Return 0 on success
	ret
read_failed:
	ld a, 1                        ; Oops, we failed return 1
	ret

sectranf:
	ld h, b                        ; There is no sector translation, just return HL = BC
	ld l, c
	ret

movef:
	ex  de,hl
	ldir                           ; Copy HL -> DE
	ex  de,hl
	ret

stub1:                             ; Unimplemented BIOS function
	ld a, $ff
	ret

initialize_disk:
	push af
	push bc
	push de
	push hl
	ld  c, $CF                     ; Controller Status Port for Slot 1
	ld  b, $04
scan_hdd:                          ; Scan each slot to find the slot with the HDD card
	in  a, (c)                     ; Read from current status register
	cp  $e8                        ; If $E8 is returned we are a HDD controller
	jr  z, found_hdd
	ld  a, c
	add a, $10                     ; Add 16 to status register
	ld  c, a
	djnz scan_hdd                  ; and try the next slot.
	halt                           ; No HDD controller found
found_hdd:
	ld  a, c
	sub $0f                        ; Keep the base base register for accessing the drive we founde
	ld  b, a
	ld  ix, drive_ports
	ld  a, b
	ld  (ix+DATAREG), a            ; Data Register
	inc a
	ld  (ix+ERRREG), a             ; Error Register
	ld  (ix+WRITEPCREG), a         ; Write precomp
	inc a
	ld  (ix+SECTORCREG), a         ; Sector Count
	inc a
	ld  (ix+SECTORNREG), a         ; Sector Number
	inc a
	ld  (ix+CYLLOREG), a           ; Cylinder Low
	inc a
	ld  (ix+CYLHIREG), a           ; Cylinder High
	inc a
	ld  (ix+SDHREG), a             ; SDH
	inc a
	ld  (ix+STATUSREG), a          ; Status Register
	ld  (ix+CMDREG), a             ; Command Register
	ld  (ix+DRIVEREG), c           ; Drive register
	xor a
	ld  (ix+TRACKLO), a            ; Initialize track, sector and DMA to 0
	ld  (ix+TRACKHI), a
	ld  (ix+SECTORLO), a
	ld  (ix+SECTORHI), a
	ld  (ix+DMALO), a
	ld  (ix+DMAHI), a
	call drive_restore             ; Recalibrate drive to track 0
	pop	hl
	pop de
	pop bc
	pop af
	ret


drive_restore:
	ld a, $20                      ; no crc, 512 byte sectors, drive 1, head 0
	ld c, (ix + SDHREG)
	out (c), a
	ld a, $15                      ; $15 = Restore command
	ld c, (ix + CMDREG)
	out (c), a                     ; Execute restore
	call drive_wait_nbusy          ; Wait till drive is not busy
	ret

drive_seek:
	call drive_load_chs            ; Convert CP/M track, sector to drive CHS values

	ld a, $75                      ; $75 = seek command
	ld c, (ix + CMDREG)
	out (c), a                     ; Execute seek
	call drive_wait_nbusy          ; Wait till drive is not busy
	ret

drive_read:
	ld  hl, (dma)                  ; Load DMA addr into HL
	ld a, $20                      ; $20 = read (single sector) command
	ld c, (ix + CMDREG)
	out (c), a                     ; Execute read

	call drive_wait_nbusy          ; Wait till drive is not busy
	ret nz                         ; return if error
	ld b, 00                       ; count is 256 bytes
	ld c, (ix+0)
	inir                           ; Read 256 bytes
	inir                           ; Read next 256 bytes
	call drive_wait_nbusy          ; Wait till drive is not busy
	ret

drive_load_chs:
	ld hl, (track)                 ; HL = track
	ld a, (ix + SECTORLO)          ; A = Sector (lo)
	ld b, (ix + SECTORHI)          ; B = Sector (hi)
	sra b                          ; Shift BA right by 4 (divide by 8)
	rr  a                          ; The top 3 bits of the sector
	sra b                          ; represent the drive head to use
	rr  a
	sra b
	rr  a
	sra b
	rr  a
	or  $20                       ; Set 512 byte sectors.
	ld  c, (ix + SDHREG)
	out (c), a                    ; Store Drive parameters/head into SDH
	ld  a, (ix + SECTORLO)
	and $0f                       ; Take the 4 LSB of sector as the actual drive sector no.
	ld c, (ix + SECTORNREG)
	out (c), a                    ; and store it in the sector register
	ld  a, $01                    ; The sector count is always one.
	ld  c, (ix + SECTORCREG)
	out (c), a
	ld  a, l                      ; Store the low byte of the track value in the cylinder (low) register
	ld  c, (ix + CYLLOREG)
	out (c), a
	ld  a, h                      ; Store the high byte of the track value in the cylinder (high) register
	ld  c, (ix + CYLHIREG)
	out (c), a
	ret

drive_wait_nbusy:
	ld c, (ix + 8)                 ; Status Port
	in a, (c)                      ; Read Status Port
	bit 7, a                       ; Check bit 7 (BSY bit)
	jr nz, drive_wait_nbusy        ; While bit 7 is set
	bit 0, a                       ; test error bit, resets Z flag on error, sets Z flag on success
	ret

drive_ports:
	ds 11
track:
	dw $0000
sector:
	dw $0000
dma:
	dw $0000

dph:
	dw $0000
	dw $0000
	dw $0000
	dw $0000
	dw $0000
	db $00
	db $00
	dw dpb
	dw $0000
	dw alv
	dw dirbcb
	dw dtabcb
	dw $ffff
	db $00

alv:
	ds 490

dirbcb:
	db $ff
	db 0,0,0
	db 0
	db 0
	dw $0000
	dw $0000
	dw $8000                       ; Directory buffer address
	db 0
	dw $0000

dtabcb:
	db $ff
	db 0,0,0
	db 0
	db 0
	dw $0000
	dw $0000
	dw $8400                       ; Data buffer address
	db 0
	dw $0000

dpb:
	dw $100                        ; spt = 256
	db $05                         ; BSH = 6, BLM = 32
	db $1f                         ; 4k Block size
	db $01                         ; EXM = 1
	dw $79f                        ; DSM = 1951
	dw $3ff                        ; DRM = 1023
	db $ff                         ; AL0
	db $00                         ; AL1
	dw $8000                       ; CKS (Fixed, no checksum vector)
	dw $01                         ; 1 System Track
	db $02                         ; PHS = 2, PHM = 3
	db $03                         ; 512 byte sectors
