static bool is_string_insn(struct insn *insn)
{
	insn_get_opcode(insn);

	 
	if (insn->opcode.nbytes != 1)
		return false;

	switch (insn->opcode.bytes[0]) {
	case 0x6c ... 0x6f:	 
	case 0xa4 ... 0xa7:	 
	case 0xaa ... 0xaf:	 
		return true;
	default:
		return false;
	}
}
