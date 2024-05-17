ssh_keysign(struct sshkey *key, u_char **sigp, size_t *lenp,
    const u_char *data, size_t datalen)
{
	struct sshbuf *b;
	struct stat st;
	pid_t pid;
	int i, r, to[2], from[2], status, sock = packet_get_connection_in();
	u_char rversion = 0, version = 2;
	void (*osigchld)(int);

	*sigp = NULL;
	*lenp = 0;

	if (stat(_PATH_SSH_KEY_SIGN, &st) < 0) {
		error("%s: not installed: %s", __func__, strerror(errno));
		return -1;
	}
	if (fflush(stdout) != 0) {
		error("%s: fflush: %s", __func__, strerror(errno));
		return -1;
	}
	if (pipe(to) < 0) {
		error("%s: pipe: %s", __func__, strerror(errno));
		return -1;
	}
	if (pipe(from) < 0) {
		error("%s: pipe: %s", __func__, strerror(errno));
		return -1;
	}
	if ((pid = fork()) < 0) {
		error("%s: fork: %s", __func__, strerror(errno));
		return -1;
	}
	osigchld = signal(SIGCHLD, SIG_DFL);
	if (pid == 0) {
		 
		fcntl(sock, F_SETFD, 0);
		permanently_drop_suid(getuid());
		close(from[0]);
		if (dup2(from[1], STDOUT_FILENO) < 0)
			fatal("%s: dup2: %s", __func__, strerror(errno));
		close(to[1]);
		if (dup2(to[0], STDIN_FILENO) < 0)
			fatal("%s: dup2: %s", __func__, strerror(errno));
		close(from[1]);
		close(to[0]);
		 
		for (i = STDERR_FILENO + 1; i < sock; i++)
			close(i);
		closefrom(sock + 1);
		debug3("%s: [child] pid=%ld, exec %s",
		    __func__, (long)getpid(), _PATH_SSH_KEY_SIGN);
		execl(_PATH_SSH_KEY_SIGN, _PATH_SSH_KEY_SIGN, (char *)NULL);
		fatal("%s: exec(%s): %s", __func__, _PATH_SSH_KEY_SIGN,
		    strerror(errno));
	}
	close(from[1]);
	close(to[0]);

	if ((b = sshbuf_new()) == NULL)
		fatal("%s: sshbuf_new failed", __func__);
	 
	if ((r = sshbuf_put_u32(b, sock) != 0) ||
	    (r = sshbuf_put_string(b, data, datalen)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));
	if (ssh_msg_send(to[1], version, b) == -1)
		fatal("%s: couldn't send request", __func__);
	sshbuf_reset(b);
	r = ssh_msg_recv(from[0], b);
	close(from[0]);
	close(to[1]);
	if (r < 0) {
		error("%s: no reply", __func__);
		goto fail;
	}

	errno = 0;
	while (waitpid(pid, &status, 0) < 0) {
		if (errno != EINTR) {
			error("%s: waitpid %ld: %s",
			    __func__, (long)pid, strerror(errno));
			goto fail;
		}
	}
	if (!WIFEXITED(status)) {
		error("%s: exited abnormally", __func__);
		goto fail;
	}
	if (WEXITSTATUS(status) != 0) {
		error("%s: exited with status %d",
		    __func__, WEXITSTATUS(status));
		goto fail;
	}
	if ((r = sshbuf_get_u8(b, &rversion)) != 0) {
		error("%s: buffer error: %s", __func__, ssh_err(r));
		goto fail;
	}
	if (rversion != version) {
		error("%s: bad version", __func__);
		goto fail;
	}
	if ((r = sshbuf_get_string(b, sigp, lenp)) != 0) {
		error("%s: buffer error: %s", __func__, ssh_err(r));
 fail:
		signal(SIGCHLD, osigchld);
		sshbuf_free(b);
		return -1;
	}
	signal(SIGCHLD, osigchld);
	sshbuf_free(b);

	return 0;
}
