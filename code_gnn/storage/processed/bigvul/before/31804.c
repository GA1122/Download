void signal_wake_up_state(struct task_struct *t, unsigned int state)
{
	set_tsk_thread_flag(t, TIF_SIGPENDING);
	 
	if (!wake_up_state(t, state | TASK_INTERRUPTIBLE))
		kick_process(t);
}
