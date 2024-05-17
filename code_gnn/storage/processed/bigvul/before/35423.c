void math_state_restore(void)
{
	struct task_struct *tsk = current;

	if (!tsk_used_math(tsk)) {
		local_irq_enable();
		 
		if (init_fpu(tsk)) {
			 
			do_group_exit(SIGKILL);
			return;
		}
		local_irq_disable();
	}

	__thread_fpu_begin(tsk);

	 
	if (unlikely(restore_fpu_checking(tsk))) {
		drop_init_fpu(tsk);
		force_sig_info(SIGSEGV, SEND_SIG_PRIV, tsk);
		return;
	}

	tsk->thread.fpu_counter++;
}
