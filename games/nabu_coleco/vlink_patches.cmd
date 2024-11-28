ENTRY(entry);

SECTIONS {
	.data 0x5000 : AT(.) {*(.data .data.*)}
}
