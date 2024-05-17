static void _6502_anal_update_flags(RAnalOp *op, int flags) {
	 
	if (flags & _6502_FLAGS_B) r_strbuf_append (&op->esil, ",$b9,C,=");
	if (flags & _6502_FLAGS_C) r_strbuf_append (&op->esil, ",$c7,C,=");
	if (flags & _6502_FLAGS_Z) r_strbuf_append (&op->esil, ",$z,Z,=");
	if (flags & _6502_FLAGS_N) r_strbuf_append (&op->esil, ",$s,N,=");
}