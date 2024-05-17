notify_fifo_exec(thread_master_t *m, int (*func) (thread_t *), void *arg, notify_script_t *script)
{
	pid_t pid;
	int retval;
	char *scr;

	pid = local_fork();

	 
	if (pid < 0) {
		log_message(LOG_INFO, "Failed fork process");
		return -1;
	}

	 
	if (pid) {
		thread_add_child(m, func, arg, pid, TIMER_NEVER);
		return 0;
	}

#ifdef _MEM_CHECK_
	skip_mem_dump();
#endif

	setpgid(0, 0);
	set_privileges(script->uid, script->gid);

	if (script->flags | SC_EXECABLE) {
		 
		prctl(PR_SET_PDEATHSIG, SIGTERM);

		execve(script->args[0], script->args, environ);

		if (errno == EACCES)
			log_message(LOG_INFO, "FIFO notify script %s is not executable", script->args[0]);
		else
			log_message(LOG_INFO, "Unable to execute FIFO notify script %s - errno %d - %m", script->args[0], errno);
	}
	else {
		retval = system(scr = cmd_str(script));

		if (retval == 127) {
			 
			log_message(LOG_ALERT, "Couldn't exec FIFO command: %s", scr);
		}
		else if (retval == -1)
			log_message(LOG_ALERT, "Error exec-ing FIFO command: %s", scr);

		exit(0);
	}

	 
	exit(0);
}