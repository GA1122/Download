asmlinkage void do_ri(struct pt_regs *regs)
{
	unsigned int __user *epc = (unsigned int __user *)exception_epc(regs);
	unsigned long old_epc = regs->cp0_epc;
	unsigned int opcode = 0;
	int status = -1;

	if (notify_die(DIE_RI, "RI Fault", regs, 0, regs_to_trapnr(regs), SIGILL)
	    == NOTIFY_STOP)
		return;

	die_if_kernel("Reserved instruction in kernel code", regs);

	if (unlikely(compute_return_epc(regs) < 0))
		return;

	if (unlikely(get_user(opcode, epc) < 0))
		status = SIGSEGV;

	if (!cpu_has_llsc && status < 0)
		status = simulate_llsc(regs, opcode);

	if (status < 0)
		status = simulate_rdhwr(regs, opcode);

	if (status < 0)
		status = simulate_sync(regs, opcode);

	if (status < 0)
		status = SIGILL;

	if (unlikely(status > 0)) {
		regs->cp0_epc = old_epc;		 
		force_sig(status, current);
	}
}