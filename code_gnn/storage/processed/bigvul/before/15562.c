control_persist_detach(void)
{
	pid_t pid;
	int devnull;

	debug("%s: backgrounding master process", __func__);

 	 
	switch ((pid = fork())) {
	case -1:
		fatal("%s: fork: %s", __func__, strerror(errno));
	case 0:
		 
 		break;
 	default:
		 
		debug2("%s: background process is %ld", __func__, (long)pid);
		stdin_null_flag = ostdin_null_flag;
		options.request_tty = orequest_tty;
		tty_flag = otty_flag;
 		close(muxserver_sock);
 		muxserver_sock = -1;
		options.control_master = SSHCTL_MASTER_NO;
 		muxclient(options.control_path);
		 
 		fatal("Failed to connect to new control master");
 	}
	if ((devnull = open(_PATH_DEVNULL, O_RDWR)) == -1) {
		error("%s: open(\"/dev/null\"): %s", __func__,
		    strerror(errno));
	} else {
		if (dup2(devnull, STDIN_FILENO) == -1 ||
		    dup2(devnull, STDOUT_FILENO) == -1)
			error("%s: dup2: %s", __func__, strerror(errno));
		if (devnull > STDERR_FILENO)
			close(devnull);
	}
	daemon(1, 1);
	setproctitle("%s [mux]", options.control_path);
}
