static int eligible_child(struct wait_opts *wo, struct task_struct *p)
{
	if (!eligible_pid(wo, p))
		return 0;
	 
	if (((p->exit_signal != SIGCHLD) ^ !!(wo->wo_flags & __WCLONE))
	    && !(wo->wo_flags & __WALL))
		return 0;

	return 1;
}
