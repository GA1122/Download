static int ptrace_write_dr7(struct task_struct *tsk, unsigned long data)
{
	struct thread_struct *thread = &(tsk->thread);
	unsigned long old_dr7;
	int i, orig_ret = 0, rc = 0;
	int enabled, second_pass = 0;
	unsigned len, type;
	struct perf_event *bp;

	if (ptrace_get_breakpoints(tsk) < 0)
		return -ESRCH;

	data &= ~DR_CONTROL_RESERVED;
	old_dr7 = ptrace_get_dr7(thread->ptrace_bps);
restore:
	 
	for (i = 0; i < HBP_NUM; i++) {
		enabled = decode_dr7(data, i, &len, &type);
		bp = thread->ptrace_bps[i];

		if (!enabled) {
			if (bp) {
				 
				if (!second_pass)
					continue;

				rc = ptrace_modify_breakpoint(bp, len, type,
							      tsk, 1);
				if (rc)
					break;
			}
			continue;
		}

		rc = ptrace_modify_breakpoint(bp, len, type, tsk, 0);
		if (rc)
			break;
	}
	 
	if (!second_pass) {
		second_pass = 1;
		if (rc < 0) {
			orig_ret = rc;
			data = old_dr7;
		}
		goto restore;
	}

	ptrace_put_breakpoints(tsk);

	return ((orig_ret < 0) ? orig_ret : rc);
}
