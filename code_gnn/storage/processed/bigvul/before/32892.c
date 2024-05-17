void mm_release(struct task_struct *tsk, struct mm_struct *mm)
{
	 
#ifdef CONFIG_FUTEX
	if (unlikely(tsk->robust_list)) {
		exit_robust_list(tsk);
		tsk->robust_list = NULL;
	}
#ifdef CONFIG_COMPAT
	if (unlikely(tsk->compat_robust_list)) {
		compat_exit_robust_list(tsk);
		tsk->compat_robust_list = NULL;
	}
#endif
	if (unlikely(!list_empty(&tsk->pi_state_list)))
		exit_pi_state_list(tsk);
#endif

	uprobe_free_utask(tsk);

	 
	deactivate_mm(tsk, mm);

	 
	if (tsk->clear_child_tid) {
		if (!(tsk->flags & PF_SIGNALED) &&
		    atomic_read(&mm->mm_users) > 1) {
			 
			put_user(0, tsk->clear_child_tid);
			sys_futex(tsk->clear_child_tid, FUTEX_WAKE,
					1, NULL, NULL, 0);
		}
		tsk->clear_child_tid = NULL;
	}

	 
	if (tsk->vfork_done)
		complete_vfork_done(tsk);
}
