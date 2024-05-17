unsigned long perf_instruction_pointer(struct pt_regs *regs)
{
	unsigned long ip;

	if (TRAP(regs) != 0xf00)
		return regs->nip;	 

	ip = mfspr(SPRN_SIAR) + perf_ip_adjust(regs);
	return ip;
}
