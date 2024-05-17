perf_callchain_user32(struct pt_regs *regs, struct perf_callchain_entry *entry)
{
	 
	struct stack_frame_ia32 frame;
	const void __user *fp;

	if (!test_thread_flag(TIF_IA32))
		return 0;

	fp = compat_ptr(regs->bp);
	while (entry->nr < PERF_MAX_STACK_DEPTH) {
		unsigned long bytes;
		frame.next_frame     = 0;
		frame.return_address = 0;

		bytes = copy_from_user_nmi(&frame, fp, sizeof(frame));
		if (bytes != sizeof(frame))
			break;

		if (fp < compat_ptr(regs->sp))
			break;

		perf_callchain_store(entry, frame.return_address);
		fp = compat_ptr(frame.next_frame);
	}
	return 1;
}
