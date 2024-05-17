static int ptrace_resume(struct task_struct *child, long request,
			 unsigned long data)
{
	bool need_siglock;

	if (!valid_signal(data))
		return -EIO;

	if (request == PTRACE_SYSCALL)
		set_tsk_thread_flag(child, TIF_SYSCALL_TRACE);
	else
		clear_tsk_thread_flag(child, TIF_SYSCALL_TRACE);

#ifdef TIF_SYSCALL_EMU
	if (request == PTRACE_SYSEMU || request == PTRACE_SYSEMU_SINGLESTEP)
		set_tsk_thread_flag(child, TIF_SYSCALL_EMU);
	else
		clear_tsk_thread_flag(child, TIF_SYSCALL_EMU);
#endif

	if (is_singleblock(request)) {
		if (unlikely(!arch_has_block_step()))
			return -EIO;
		user_enable_block_step(child);
	} else if (is_singlestep(request) || is_sysemu_singlestep(request)) {
		if (unlikely(!arch_has_single_step()))
			return -EIO;
		user_enable_single_step(child);
	} else {
		user_disable_single_step(child);
	}

	 
	need_siglock = data && !thread_group_empty(current);
	if (need_siglock)
		spin_lock_irq(&child->sighand->siglock);
	child->exit_code = data;
	wake_up_state(child, __TASK_TRACED);
	if (need_siglock)
		spin_unlock_irq(&child->sighand->siglock);

	return 0;
}