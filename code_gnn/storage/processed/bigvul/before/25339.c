static int thumb2_break_trap(struct pt_regs *regs, unsigned int instr)
{
	unsigned int instr2;
	void __user *pc;

	 
	pc = (void __user *)(instruction_pointer(regs) + 2);

	if (processor_mode(regs) == SVC_MODE) {
		instr2 = *(u16 *) pc;
	} else {
		get_user(instr2, (u16 __user *)pc);
	}

	if (instr2 == 0xa000) {
		ptrace_break(current, regs);
		return 0;
	} else {
		return 1;
	}
}
