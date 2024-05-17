static void perf_callchain_user_32(struct perf_callchain_entry *entry,
				   struct pt_regs *regs)
{
	unsigned long ufp;

	perf_callchain_store(entry, regs->tpc);

	ufp = regs->u_regs[UREG_I6] & 0xffffffffUL;
	do {
		struct sparc_stackf32 *usf, sf;
		unsigned long pc;

		usf = (struct sparc_stackf32 *) ufp;
		if (__copy_from_user_inatomic(&sf, usf, sizeof(sf)))
			break;

		pc = sf.callers_pc;
		ufp = (unsigned long)sf.fp;
		perf_callchain_store(entry, pc);
	} while (entry->nr < PERF_MAX_STACK_DEPTH);
}
