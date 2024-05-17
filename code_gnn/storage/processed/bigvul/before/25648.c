perf_callchain_user(struct perf_callchain_entry *entry, struct pt_regs *regs)
{
	flushw_user();
	if (test_thread_flag(TIF_32BIT))
		perf_callchain_user_32(entry, regs);
	else
		perf_callchain_user_64(entry, regs);
}
