static int wait_consider_task(struct task_struct *parent, int ptrace,
			      struct task_struct *p, int *notask_error,
			      enum pid_type type, struct pid *pid, int options,
			      struct siginfo __user *infop,
			      int __user *stat_addr, struct rusage __user *ru)
{
	int ret = eligible_child(type, pid, options, p);
	if (!ret)
		return ret;

	if (unlikely(ret < 0)) {
		 
		if (*notask_error)
			*notask_error = ret;
	}

	if (likely(!ptrace) && unlikely(p->ptrace)) {
		 
		*notask_error = 0;
		return 0;
	}

	if (p->exit_state == EXIT_DEAD)
		return 0;

	 
	if (p->exit_state == EXIT_ZOMBIE && !delay_group_leader(p))
		return wait_task_zombie(p, options, infop, stat_addr, ru);

	 
	*notask_error = 0;

	if (task_is_stopped_or_traced(p))
		return wait_task_stopped(ptrace, p, options,
					 infop, stat_addr, ru);

	return wait_task_continued(p, options, infop, stat_addr, ru);
}
