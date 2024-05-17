void sig_int(int sig_num)
{
	 
	msleep(400);

	 
	if (am_daemon && !am_server && sig_num == SIGTERM)
		exit_cleanup(0);

	 
	if (!got_kill_signal && (am_server || am_receiver)) {
		got_kill_signal = sig_num;
		return;
	}

	exit_cleanup(RERR_SIGNAL);
}
