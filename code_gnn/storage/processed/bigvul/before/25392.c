asmlinkage void do_bp(struct pt_regs *regs)
{
	unsigned int opcode, bcode;

	if (__get_user(opcode, (unsigned int __user *) exception_epc(regs)))
		goto out_sigsegv;

	 
	bcode = ((opcode >> 6) & ((1 << 20) - 1));
	if (bcode >= (1 << 10))
		bcode >>= 10;

	 
	switch (bcode) {
	case BRK_KPROBE_BP:
		if (notify_die(DIE_BREAK, "debug", regs, bcode, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
			return;
		else
			break;
	case BRK_KPROBE_SSTEPBP:
		if (notify_die(DIE_SSTEPBP, "single_step", regs, bcode, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
			return;
		else
			break;
	default:
		break;
	}

	do_trap_or_bp(regs, bcode, "Break");
	return;

out_sigsegv:
	force_sig(SIGSEGV, current);
}
