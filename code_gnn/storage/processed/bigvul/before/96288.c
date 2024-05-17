nfs_check_exportfs(void)
{
	pid_t pid;
	int rc, status;
	static char nfs_exportfs_tempfile[] = "/tmp/exportfs.XXXXXX";

	 
	if (nfs_exportfs_temp_fd >= 0)
		close(nfs_exportfs_temp_fd);

	nfs_exportfs_temp_fd = mkstemp(nfs_exportfs_tempfile);

	if (nfs_exportfs_temp_fd < 0)
		return (SA_SYSTEM_ERR);

	unlink(nfs_exportfs_tempfile);

	fcntl(nfs_exportfs_temp_fd, F_SETFD, FD_CLOEXEC);

	pid = fork();

	if (pid < 0) {
		(void) close(nfs_exportfs_temp_fd);
		nfs_exportfs_temp_fd = -1;
		return (SA_SYSTEM_ERR);
	}

	if (pid > 0) {
		while ((rc = waitpid(pid, &status, 0)) <= 0 && errno == EINTR);

		if (rc <= 0) {
			(void) close(nfs_exportfs_temp_fd);
			nfs_exportfs_temp_fd = -1;
			return (SA_SYSTEM_ERR);
		}

		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
			(void) close(nfs_exportfs_temp_fd);
			nfs_exportfs_temp_fd = -1;
			return (SA_CONFIG_ERR);
		}

		return (SA_OK);
	}

	 

	 

	if (dup2(nfs_exportfs_temp_fd, STDOUT_FILENO) < 0)
		exit(1);

	rc = execlp("/usr/sbin/exportfs", "exportfs", "-v", NULL);

	if (rc < 0) {
		exit(1);
	}

	exit(0);
}
