ENTRY(entry);

SECTIONS {
	.crt0 0x140d : {
		*(.crt0 .crt0.*)
	}
	.text 0x2110 : AT(.)  {*(.text .text.*)}
	.data : {*(.data .data.*)}
	PROVIDE(reloc_end = LOADADDR(.data) + SIZEOF(.data));
	PROVIDE(reloc_length = SIZEOF(.text) + SIZEOF(.data));
	PROVIDE(reloc_start = 0x2110 + reloc_length - 1);
}
