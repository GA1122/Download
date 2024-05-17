static int wait_consider_task(struct wait_opts *wo, int ptrace,
				struct task_struct *p)
{
	 
	int exit_state = ACCESS_ONCE(p->exit_state);
	int ret;

	if (unlikely(exit_state == EXIT_DEAD))
		return 0;

	ret = eligible_child(wo, ptrace, p);
	if (!ret)
		return ret;

	if (unlikely(exit_state == EXIT_TRACE)) {
		 
		if (likely(!ptrace))
			wo->notask_error = 0;
		return 0;
	}

	if (likely(!ptrace) && unlikely(p->ptrace)) {
		 
		if (!ptrace_reparented(p))
			ptrace = 1;
	}

	 
	if (exit_state == EXIT_ZOMBIE) {
		 
		if (!delay_group_leader(p)) {
			 
			if (unlikely(ptrace) || likely(!p->ptrace))
				return wait_task_zombie(wo, p);
		}

		 
		if (likely(!ptrace) || (wo->wo_flags & (WCONTINUED | WEXITED)))
			wo->notask_error = 0;
	} else {
		 
		wo->notask_error = 0;
	}

	 
	ret = wait_task_stopped(wo, ptrace, p);
	if (ret)
		return ret;

	 
	return wait_task_continued(wo, p);
}
