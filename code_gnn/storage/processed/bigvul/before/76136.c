set_privileges(uid_t uid, gid_t gid)
{
	int retval;

	 
	prctl(PR_SET_PDEATHSIG, SIGTERM);

	 
	if (cur_prio != INT_MAX)
		cur_prio = getpriority(PRIO_PROCESS, 0);
	if (cur_prio < 0)
		setpriority(PRIO_PROCESS, 0, 0);

	 
	if (gid) {
		retval = setgid(gid);
		if (retval < 0) {
			log_message(LOG_ALERT, "Couldn't setgid: %d (%m)", gid);
			return true;
		}

		 
		retval = setgroups(1, &gid);
		if (retval < 0) {
			log_message(LOG_ALERT, "Couldn't setgroups: %d (%m)", gid);
			return true;
		}
	}

	if (uid) {
		retval = setuid(uid);
		if (retval < 0) {
			log_message(LOG_ALERT, "Couldn't setuid: %d (%m)", uid);
			return true;
		}
	}

	 
	signal_handler_script();
	set_std_fd(false);

	return false;
}
