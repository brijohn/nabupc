;
;	Startup for Nabu
;

    module  nabu_crt0 


;--------
; Include zcc_opt.def to find out some info
;--------

    defc    crt0 = 1
    INCLUDE "zcc_opt.def"

;--------
; Some scope definitions
;--------

    EXTERN  _main           ;main() is always external to crt0 code

    PUBLIC  cleanup         ;jp'd to by exit()
    PUBLIC  l_dcal          ;jp(hl)

    defc    CONSOLE_COLUMNS = 32
IF !DEFINED_CONSOLE_ROWS
    defc    CONSOLE_ROWS = 24
ENDIF


    defc    CRT_KEY_DEL = 127
    defc    __CPU_CLOCK = 3570000

    PUBLIC  PSG_AY_REG
    PUBLIC  PSG_AY_DATA
    defc    PSG_AY_REG = $40
    defc    PSG_AY_DATA = $41

    EXTERN  __CODE_head
    EXTERN  __tail
    EXTERN  cpm_platform_init
    EXTERN  vdp_set_mode

    defc    TAR__clib_exit_stack_size = 0
    defc    TAR__register_sp = $ff00
    defc    TAR__fputc_cons_generic = 1

IF !DEFINED_CRT_ORG_CODE
    defc    CRT_ORG_CODE = $c000
ENDIF

    defc    __length = __tail - __CODE_head
    defc    __phys_tail = start_phys + __length - 1

    INCLUDE "crt/classic/crt_rules.inc"

    org     0
relocator:
    defb    0,0,0
    ld sp, $2410                   ; Set Stack Pointer 
    ld a, $C9                      ; Return OPCODE
    ld ($240d), a                  ; Place return statement at address 0
    call $240d                     ; Call address (and return)
    ld hl, ($240e)                 ; Load return address from stack, this will be the address immediately following the call 0
    ld de, __phys_tail-$ + 3       ; DE = end of main program relative to the call 0 return address
    add hl, de                     ; HL = address of end of main program
    ld de, __tail-1                ; DE = address to copy main program to (in reverse)
    ld bc, __length                ; BC = length of main program code
    lddr                           ; Relocate starting from the end of program code
    ld hl, start                   ; HL = entry point
    jp (hl)                        ; Jump to entry point

start_phys:
SECTION CODE
	org CRT_ORG_CODE
start:
    ld      (__restore_sp_onexit+1),sp
    INCLUDE "crt/classic/crt_init_sp.asm"
    INCLUDE "crt/classic/crt_init_atexit.asm"
    di
    ld      a,$ff
    ld      i,a
    im      2
    call    crt0_init_bss
    ; Code is shared with CP/M. This is a noop, but pulls in code
    ; into crt0_init and crt0_exit
    call    cpm_platform_init 
    ld      (exitsp),sp

     ; Initialise mode 2 by default
    ld      hl,2
    call    vdp_set_mode


; Optional definition for auto MALLOC init
; it assumes we have free space between the end of 
; the compiled program and the stack pointer
IF DEFINED_USING_amalloc
    INCLUDE "crt/classic/crt_init_amalloc.asm"
ENDIF

    call    _main
cleanup:
    push    hl
    call    crt0_exit

    pop     bc
__restore_sp_onexit:
    ld      sp,0
    ret

l_dcal:
    jp      (hl)


    INCLUDE "crt/classic/crt_runtime_selection.asm" 
    INCLUDE	"crt/classic/crt_section.asm"
    INCLUDE "target/nabu/classic/nabu_hccabuf.asm"

