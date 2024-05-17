static inline int decode_access_size(unsigned int insn)
{
	insn = (insn >> 19) & 3;

	if(!insn)
		return 4;
	else if(insn == 3)
		return 8;
	else if(insn == 2)
		return 2;
	else {
		printk("Impossible unaligned trap. insn=%08x\n", insn);
		die_if_kernel("Byte sized unaligned access?!?!", current->thread.kregs);
		return 4;  
	}
}
