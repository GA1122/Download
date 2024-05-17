static int monitor_application(pid_t app_pid) {
	EUID_ASSERT();
	monitored_pid = app_pid;

	sigset_t oldmask, newmask;
	sigemptyset(&oldmask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGTERM);
	sigaddset(&newmask, SIGINT);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	install_handler();

	int options = 0;;
	unsigned timeout = 0;
	if (cfg.timeout) {
		options = WNOHANG;
		timeout = cfg.timeout;
	}

	int status = 0;
	while (monitored_pid) {
		usleep(20000);
		char *msg;
		if (asprintf(&msg, "monitoring pid %d\n", monitored_pid) == -1)
			errExit("asprintf");
		logmsg(msg);
		if (arg_debug)
			printf("%s\n", msg);
		free(msg);

		pid_t rv;
		do {
			sigprocmask(SIG_SETMASK, &oldmask, NULL);

			rv = waitpid(-1, &status, options);

			sigprocmask(SIG_BLOCK, &newmask, NULL);

			if (rv == -1) {  
				sleep(1);
				break;
			}

			if (options) {
				if (--timeout == 0)  {
					kill(-1, SIGTERM);
					sleep(1);
					flush_stdin();
					_exit(1);
				}
				else
					sleep(1);
			}
		}
		while(rv != monitored_pid);
		if (arg_debug)
			printf("Sandbox monitor: waitpid %d retval %d status %d\n", monitored_pid, rv, status);

		DIR *dir;
		if (!(dir = opendir("/proc"))) {
			sleep(2);
			if (!(dir = opendir("/proc"))) {
				fprintf(stderr, "Error: cannot open /proc directory\n");
				exit(1);
			}
		}

		struct dirent *entry;
		monitored_pid = 0;
		while ((entry = readdir(dir)) != NULL) {
			unsigned pid;
			if (sscanf(entry->d_name, "%u", &pid) != 1)
				continue;
			if (pid == 1)
				continue;

			int found = 0;
			if (strcmp(cfg.command_name, "dillo") == 0) {
				char *pidname = pid_proc_comm(pid);
				if (pidname && strcmp(pidname, "dpid") == 0)
					found = 1;
				free(pidname);
			}
			if (found)
				break;

			monitored_pid = pid;
			break;
		}
		closedir(dir);

		if (monitored_pid != 0 && arg_debug)
			printf("Sandbox monitor: monitoring %d\n", monitored_pid);
	}

	return status;
}
