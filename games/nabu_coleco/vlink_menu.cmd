ENTRY(entry);

SECTIONS {
	.data 0x4000 : AT(.) {*(.data .data.*)}
}

