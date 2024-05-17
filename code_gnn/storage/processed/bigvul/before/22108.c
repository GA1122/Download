static int eligible_child(enum pid_type type, struct pid *pid, int options,
			  struct task_struct *p)
{
	int err;

	if (type < PIDTYPE_MAX) {
		if (task_pid_type(p, type) != pid)
			return 0;
	}

	 
	if (((p->exit_signal != SIGCHLD) ^ ((options & __WCLONE) != 0))
	    && !(options & __WALL))
		return 0;

	err = security_task_wait(p);
	if (err)
		return err;

	return 1;
}
