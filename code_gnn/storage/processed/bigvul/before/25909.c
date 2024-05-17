int ptrace_set_debugreg(struct task_struct *tsk, int n, unsigned long val)
{
	struct thread_struct *thread = &(tsk->thread);
	int rc = 0;

	 
	if (n == 4 || n == 5)
		return -EIO;

	if (n == 6) {
		thread->debugreg6 = val;
		goto ret_path;
	}
	if (n < HBP_NUM) {
		rc = ptrace_set_breakpoint_addr(tsk, n, val);
		if (rc)
			return rc;
	}
	 
	if (n == 7) {
		rc = ptrace_write_dr7(tsk, val);
		if (!rc)
			thread->ptrace_dr7 = val;
	}

ret_path:
	return rc;
}
