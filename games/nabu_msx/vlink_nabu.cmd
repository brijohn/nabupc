ENTRY(entry);

SECTIONS {
	.crt0 0x140d : {
		*(.crt0)
		. = ALIGN(256);
	}
	PROVIDE(IM2_PAGE = (. + 1) >> 8);
	.vector : {*(.int_vec)}
	.text :   {*(.text .text.*)}
	.rodata : {*(.rodata .rodata.*)}
	.data :   {*(.data .data.*)}
	PROVIDE(msxcode = .);
	.msx  :   {*(.msx)}
	PROVIDE(msxcode_end = . );
	PROVIDE(msxlength = msxcode_end - msxcode);
}
