void signal_wake_up(struct task_struct *t, int resume)
{
	unsigned int mask;

	set_tsk_thread_flag(t, TIF_SIGPENDING);

	 
	mask = TASK_INTERRUPTIBLE;
	if (resume)
		mask |= TASK_WAKEKILL;
	if (!wake_up_state(t, mask))
		kick_process(t);
}
