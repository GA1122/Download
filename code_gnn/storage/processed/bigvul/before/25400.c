asmlinkage void do_reserved(struct pt_regs *regs)
{
	 
	show_regs(regs);
	panic("Caught reserved exception %ld - should not happen.",
	      (regs->cp0_cause & 0x7f) >> 2);
}
