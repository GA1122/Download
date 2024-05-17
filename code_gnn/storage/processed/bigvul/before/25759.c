perf_callchain_user(struct perf_callchain_entry *entry, struct pt_regs *regs)
{
	struct stack_frame frame;
	const void __user *fp;

	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
		 
		return;
	}

	fp = (void __user *)regs->bp;

	perf_callchain_store(entry, regs->ip);

	if (perf_callchain_user32(regs, entry))
		return;

	while (entry->nr < PERF_MAX_STACK_DEPTH) {
		unsigned long bytes;
		frame.next_frame	     = NULL;
		frame.return_address = 0;

		bytes = copy_from_user_nmi(&frame, fp, sizeof(frame));
		if (bytes != sizeof(frame))
			break;

		if ((unsigned long)fp < regs->sp)
			break;

		perf_callchain_store(entry, frame.return_address);
		fp = frame.next_frame;
	}
}