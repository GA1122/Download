int is_dsp_inst(struct pt_regs *regs)
{
	unsigned short inst = 0;

	 
	if (!(current_cpu_data.flags & CPU_HAS_DSP) || (regs->sr & SR_DSP))
		return 0;

	get_user(inst, ((unsigned short *) regs->pc));

	inst &= 0xf000;

	 
	if ((inst == 0xf000) || (inst == 0x4000))
		return 1;

	return 0;
}
