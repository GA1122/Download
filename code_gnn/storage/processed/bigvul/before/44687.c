extern struct lxc_popen_FILE *lxc_popen(const char *command)
{
	struct lxc_popen_FILE *fp = NULL;
	int parent_end = -1, child_end = -1;
	int pipe_fds[2];
	pid_t child_pid;

	int r = pipe2(pipe_fds, O_CLOEXEC);

	if (r < 0) {
		ERROR("pipe2 failure");
		return NULL;
	}

	parent_end = pipe_fds[0];
	child_end = pipe_fds[1];

	child_pid = fork();

	if (child_pid == 0) {
		 
		int child_std_end = STDOUT_FILENO;

		if (child_end != child_std_end) {
			 
			dup2(child_end, child_std_end);

			 
		} else {
			 
			if (fcntl(child_end, F_SETFD, 0) != 0) {
				SYSERROR("Failed to remove FD_CLOEXEC from fd.");
				exit(127);
			}
		}

		 
		{
			sigset_t mask;
			sigfillset(&mask);
			sigprocmask(SIG_UNBLOCK, &mask, NULL);
		}

		execl("/bin/sh", "sh", "-c", command, (char *) NULL);
		exit(127);
	}

	 

	close(child_end);
	child_end = -1;

	if (child_pid < 0) {
		ERROR("fork failure");
		goto error;
	}

	fp = calloc(1, sizeof(*fp));
	if (!fp) {
		ERROR("failed to allocate memory");
		goto error;
	}

	fp->f = fdopen(parent_end, "r");
	if (!fp->f) {
		ERROR("fdopen failure");
		goto error;
	}

	fp->child_pid = child_pid;

	return fp;

error:

	if (fp) {
		if (fp->f) {
			fclose(fp->f);
			parent_end = -1;  
		}

		free(fp);
	}

	if (parent_end != -1)
		close(parent_end);

	return NULL;
}
