set_control_persist_exit_time(void)
{
	if (muxserver_sock == -1 || !options.control_persist
	    || options.control_persist_timeout == 0) {
		 
		control_persist_exit_time = 0;
	} else if (channel_still_open()) {
		 
		if (control_persist_exit_time > 0)
			debug2("%s: cancel scheduled exit", __func__);
		control_persist_exit_time = 0;
	} else if (control_persist_exit_time <= 0) {
		 
		control_persist_exit_time = monotime() +
			(time_t)options.control_persist_timeout;
		debug2("%s: schedule exit in %d seconds", __func__,
		    options.control_persist_timeout);
	}
	 
}