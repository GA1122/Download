static void perf_callchain_user_64(struct perf_callchain_entry *entry,
				   struct pt_regs *regs)
{
	unsigned long ufp;

	perf_callchain_store(entry, regs->tpc);

	ufp = regs->u_regs[UREG_I6] + STACK_BIAS;
	do {
		struct sparc_stackf *usf, sf;
		unsigned long pc;

		usf = (struct sparc_stackf *) ufp;
		if (__copy_from_user_inatomic(&sf, usf, sizeof(sf)))
			break;

		pc = sf.callers_pc;
		ufp = (unsigned long)sf.fp + STACK_BIAS;
		perf_callchain_store(entry, pc);
	} while (entry->nr < PERF_MAX_STACK_DEPTH);
}
