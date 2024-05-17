script_killall(thread_master_t *m, int signo, bool requeue)
{
	thread_t *thread;
	pid_t p_pgid, c_pgid;
#ifndef HAVE_SIGNALFD
	sigset_t old_set, child_wait;

	sigmask_func(0, NULL, &old_set);
	if (!sigismember(&old_set, SIGCHLD)) {
		sigemptyset(&child_wait);
		sigaddset(&child_wait, SIGCHLD);
		sigmask_func(SIG_BLOCK, &child_wait, NULL);
	}
#endif

	p_pgid = getpgid(0);

	rb_for_each_entry_cached(thread, &m->child, n) {
		c_pgid = getpgid(thread->u.c.pid);
		if (c_pgid != p_pgid)
			kill(-c_pgid, signo);
		else {
			log_message(LOG_INFO, "Child process %d in our process group %d", c_pgid, p_pgid);
			kill(thread->u.c.pid, signo);
		}
	}

	 
	if (requeue && signo != SIGKILL)
		thread_children_reschedule(m, child_killed_thread, TIMER_HZ);

#ifndef HAVE_SIGNALFD
	if (!sigismember(&old_set, SIGCHLD))
		sigmask_func(SIG_UNBLOCK, &child_wait, NULL);
#endif
}