void altivec_assist_exception(struct pt_regs *regs)
{
	int err;

	if (!user_mode(regs)) {
		printk(KERN_EMERG "VMX/Altivec assist exception in kernel mode"
		       " at %lx\n", regs->nip);
		die("Kernel VMX/Altivec assist exception", regs, SIGILL);
	}

	flush_altivec_to_thread(current);

	err = emulate_altivec(regs);
	if (err == 0) {
		regs->nip += 4;		 
		emulate_single_step(regs);
		return;
	}

	if (err == -EFAULT) {
		 
		_exception(SIGSEGV, regs, SEGV_ACCERR, regs->nip);
	} else {
		 
		 
		if (printk_ratelimit())
			printk(KERN_ERR "Unrecognized altivec instruction "
			       "in %s at %lx\n", current->comm, regs->nip);
		current->thread.vscr.u[3] |= 0x10000;
	}
}
