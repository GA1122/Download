static int default_cu2_call(struct notifier_block *nfb, unsigned long action,
        void *data)
{
	struct pt_regs *regs = data;

	switch (action) {
	default:
		die_if_kernel("Unhandled kernel unaligned access or invalid "
			      "instruction", regs);
		 

	case CU2_EXCEPTION:
		force_sig(SIGILL, current);
	}

	return NOTIFY_OK;
}