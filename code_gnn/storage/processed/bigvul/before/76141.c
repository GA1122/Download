system_call(const notify_script_t* script)
{
	char *command_line = NULL;
	char *str;
	int retval;

	if (set_privileges(script->uid, script->gid))
		exit(0);

	 
	setpgid(0, 0);

	if (script->flags & SC_EXECABLE) {
		 
		prctl(PR_SET_PDEATHSIG, SIGTERM);

		execve(script->args[0], script->args, environ);

		 
		log_message(LOG_ALERT, "Error exec-ing command '%s', error %d: %m", script->args[0], errno);
	}
	else {
		retval = system(str = cmd_str(script));

		if (retval == -1)
			log_message(LOG_ALERT, "Error exec-ing command: %s", str);
		else if (WIFEXITED(retval)) {
			if (WEXITSTATUS(retval) == 127) {
				 
				log_message(LOG_ALERT, "Couldn't find command: %s", str);
			}
			else if (WEXITSTATUS(retval) == 126) {
				 
				log_message(LOG_ALERT, "Couldn't execute command: %s", str);
			}
		}

		if (command_line)
			FREE(command_line);

		if (retval == -1 ||
		    (WIFEXITED(retval) && (WEXITSTATUS(retval) == 126 || WEXITSTATUS(retval) == 127)))
			exit(0);
		if (WIFEXITED(retval))
			exit(WEXITSTATUS(retval));
		if (WIFSIGNALED(retval))
			kill(getpid(), WTERMSIG(retval));
		exit(0);
	}

	exit(0);
}