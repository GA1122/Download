static void copy_seccomp(struct task_struct *p)
{
#ifdef CONFIG_SECCOMP
	 
	assert_spin_locked(&current->sighand->siglock);

	 
	get_seccomp_filter(current);
	p->seccomp = current->seccomp;

	 
	if (task_no_new_privs(current))
		task_set_no_new_privs(p);

	 
	if (p->seccomp.mode != SECCOMP_MODE_DISABLED)
		set_tsk_thread_flag(p, TIF_SECCOMP);
#endif
}
