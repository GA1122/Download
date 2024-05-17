static int sig_ignored(struct task_struct *t, int sig, int from_ancestor_ns)
{
	 
	if (sigismember(&t->blocked, sig) || sigismember(&t->real_blocked, sig))
		return 0;

	if (!sig_task_ignored(t, sig, from_ancestor_ns))
		return 0;

	 
	return !tracehook_consider_ignored_signal(t, sig);
}
