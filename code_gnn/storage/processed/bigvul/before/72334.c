wait_until_can_do_something(int connection_in, int connection_out,
    fd_set **readsetp, fd_set **writesetp, int *maxfdp,
    u_int *nallocp, u_int64_t max_time_ms)
{
	struct timeval tv, *tvp;
	int ret;
	time_t minwait_secs = 0;
	int client_alive_scheduled = 0;

	 
	channel_prepare_select(readsetp, writesetp, maxfdp, nallocp,
	    &minwait_secs, 0);

	 
	if (minwait_secs != 0)
		max_time_ms = MINIMUM(max_time_ms, (u_int)minwait_secs * 1000);

	 
	if (options.client_alive_interval) {
		uint64_t keepalive_ms =
		    (uint64_t)options.client_alive_interval * 1000;

		client_alive_scheduled = 1;
		if (max_time_ms == 0 || max_time_ms > keepalive_ms)
			max_time_ms = keepalive_ms;
	}

#if 0
	 
	if (channel_not_very_much_buffered_data())
#endif
	FD_SET(connection_in, *readsetp);
	notify_prepare(*readsetp);

	 
	if (packet_have_data_to_write())
		FD_SET(connection_out, *writesetp);

	 
	if (child_terminated && packet_not_very_much_data_to_write())
		if (max_time_ms == 0 || client_alive_scheduled)
			max_time_ms = 100;

	if (max_time_ms == 0)
		tvp = NULL;
	else {
		tv.tv_sec = max_time_ms / 1000;
		tv.tv_usec = 1000 * (max_time_ms % 1000);
		tvp = &tv;
	}

	 
	ret = select((*maxfdp)+1, *readsetp, *writesetp, NULL, tvp);

	if (ret == -1) {
		memset(*readsetp, 0, *nallocp);
		memset(*writesetp, 0, *nallocp);
		if (errno != EINTR)
			error("select: %.100s", strerror(errno));
	} else if (ret == 0 && client_alive_scheduled)
		client_alive_check();

	notify_done(*readsetp);
}
