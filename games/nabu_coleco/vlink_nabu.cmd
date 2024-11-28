ENTRY(entry);

SECTIONS {
	.crt0 0x140d : {
		*(.crt0 .crt0.*)
	}
	PROVIDE(reloc_start = .);
	.text 0x2100 : AT(.)  {*(.text .text.*)}
	.data : {*(.data .data.*)}
	PROVIDE(reloc_end = LOADADDR(.data) + SIZEOF(.data));
	PROVIDE(reloc_length = SIZEOF(.text) + SIZEOF(.data));
}
