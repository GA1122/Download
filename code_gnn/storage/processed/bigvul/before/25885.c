single_step_cont(struct pt_regs *regs, struct die_args *args)
{
	 
	printk(KERN_ERR "KGDB: trap/step from kernel to user space, "
			"resuming...\n");
	kgdb_arch_handle_exception(args->trapnr, args->signr,
				   args->err, "c", "", regs);
	 
	(*(unsigned long *)ERR_PTR(args->err)) &= ~DR_STEP;

	return NOTIFY_STOP;
}
