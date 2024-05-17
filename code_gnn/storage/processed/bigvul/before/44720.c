static int lxc_attach_to_ns(pid_t pid, int which)
{
	char path[MAXPATHLEN];
	 
	static char *ns[] = { "user", "mnt", "pid", "uts", "ipc", "net" };
	static int flags[] = {
		CLONE_NEWUSER, CLONE_NEWNS, CLONE_NEWPID, CLONE_NEWUTS, CLONE_NEWIPC,
		CLONE_NEWNET
	};
	static const int size = sizeof(ns) / sizeof(char *);
	int fd[size];
	int i, j, saved_errno;


	snprintf(path, MAXPATHLEN, "/proc/%d/ns", pid);
	if (access(path, X_OK)) {
		ERROR("Does this kernel version support 'attach' ?");
		return -1;
	}

	for (i = 0; i < size; i++) {
		 
		if (which != -1 && !(which & flags[i])) {
			fd[i] = -1;
			continue;
		}

		snprintf(path, MAXPATHLEN, "/proc/%d/ns/%s", pid, ns[i]);
		fd[i] = open(path, O_RDONLY | O_CLOEXEC);
		if (fd[i] < 0) {
			saved_errno = errno;

			 
			for (j = 0; j < i; j++)
				close(fd[j]);

			errno = saved_errno;
			SYSERROR("failed to open '%s'", path);
			return -1;
		}
	}

	for (i = 0; i < size; i++) {
		if (fd[i] >= 0 && setns(fd[i], 0) != 0) {
			saved_errno = errno;

			for (j = i; j < size; j++)
				close(fd[j]);

			errno = saved_errno;
			SYSERROR("failed to set namespace '%s'", ns[i]);
			return -1;
		}

		close(fd[i]);
	}

	return 0;
}