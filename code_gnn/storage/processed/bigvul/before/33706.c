void user_disable_single_step(struct task_struct *child)
{
	 
	if (test_tsk_thread_flag(child, TIF_BLOCKSTEP))
		set_task_blockstep(child, false);

	 
	clear_tsk_thread_flag(child, TIF_SINGLESTEP);

	 
	if (test_and_clear_tsk_thread_flag(child, TIF_FORCED_TF))
		task_pt_regs(child)->flags &= ~X86_EFLAGS_TF;
}
