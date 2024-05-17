void perf_callchain_kernel(struct perf_callchain_entry *entry,
			   struct pt_regs *regs)
{
	struct stackframe frame;

	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
		 
		return;
	}

	frame.fp = regs->regs[29];
	frame.sp = regs->sp;
	frame.pc = regs->pc;

	walk_stackframe(&frame, callchain_trace, entry);
}