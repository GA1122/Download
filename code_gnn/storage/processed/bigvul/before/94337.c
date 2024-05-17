static int wait_consider_task(struct wait_opts *wo, int ptrace,
				struct task_struct *p)
{
	int ret = eligible_child(wo, p);
	if (!ret)
		return ret;

	ret = security_task_wait(p);
	if (unlikely(ret < 0)) {
		 
		if (wo->notask_error)
			wo->notask_error = ret;
		return 0;
	}

	if (likely(!ptrace) && unlikely(task_ptrace(p))) {
		 
		wo->notask_error = 0;
		return 0;
	}

	if (p->exit_state == EXIT_DEAD)
		return 0;

	 
	if (p->exit_state == EXIT_ZOMBIE && !delay_group_leader(p))
		return wait_task_zombie(wo, p);

	 
	wo->notask_error = 0;

	if (task_stopped_code(p, ptrace))
		return wait_task_stopped(wo, ptrace, p);

	return wait_task_continued(wo, p);
}
