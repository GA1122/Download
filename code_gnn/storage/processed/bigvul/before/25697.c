static inline int decode_asi(unsigned int insn, struct pt_regs *regs)
{
	if (insn & 0x800000) {
		if (insn & 0x2000)
			return (unsigned char)(regs->tstate >> 24);	 
		else
			return (unsigned char)(insn >> 5);		 
	} else
		return ASI_P;
}
