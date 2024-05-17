collect_children(void)
{
	pid_t pid;
	sigset_t oset, nset;
	int status;

	 
	sigemptyset(&nset);
	sigaddset(&nset, SIGCHLD);
	sigprocmask(SIG_BLOCK, &nset, &oset);
	if (child_terminated) {
		debug("Received SIGCHLD.");
		while ((pid = waitpid(-1, &status, WNOHANG)) > 0 ||
		    (pid < 0 && errno == EINTR))
			if (pid > 0)
				session_close_by_pid(pid, status);
		child_terminated = 0;
	}
	sigprocmask(SIG_SETMASK, &oset, NULL);
}