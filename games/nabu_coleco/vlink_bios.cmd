ENTRY(entry);

SECTIONS {
	.text :   {*(.text .text.*)}
	.rodata : {*(.rodata .rodata.*)}
	.data :   {*(.data .data.*)}
	.ram  0x7000 (NOLOAD) : {*(.ram .ram.*)}
	.cart 0x8000 (NOLOAD) : {*(.cart .cart.*)}
}
