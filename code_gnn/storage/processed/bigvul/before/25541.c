static int set_single_step(struct task_struct *tsk, unsigned long addr)
{
	struct thread_struct *thread = &tsk->thread;
	struct perf_event *bp;
	struct perf_event_attr attr;

	bp = thread->ptrace_bps[0];
	if (!bp) {
		ptrace_breakpoint_init(&attr);

		attr.bp_addr = addr;
		attr.bp_len = HW_BREAKPOINT_LEN_2;
		attr.bp_type = HW_BREAKPOINT_R;

		bp = register_user_hw_breakpoint(&attr, ptrace_triggered, tsk);
		if (IS_ERR(bp))
			return PTR_ERR(bp);

		thread->ptrace_bps[0] = bp;
	} else {
		int err;

		attr = bp->attr;
		attr.bp_addr = addr;
		 
		attr.disabled = false;
		err = modify_user_hw_breakpoint(bp, &attr);
		if (unlikely(err))
			return err;
	}

	return 0;
}