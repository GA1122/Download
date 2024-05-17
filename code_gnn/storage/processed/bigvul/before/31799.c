static int sig_ignored(struct task_struct *t, int sig, bool force)
{
	 
	if (sigismember(&t->blocked, sig) || sigismember(&t->real_blocked, sig))
		return 0;

	if (!sig_task_ignored(t, sig, force))
		return 0;

	 
	return !t->ptrace;
}
