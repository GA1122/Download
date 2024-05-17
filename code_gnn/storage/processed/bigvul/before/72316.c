client_alive_check(void)
{
	int channel_id;

	 
	if (packet_inc_alive_timeouts() > options.client_alive_count_max) {
		logit("Timeout, client not responding.");
		cleanup_exit(255);
	}

	 
	if ((channel_id = channel_find_open()) == -1) {
		packet_start(SSH2_MSG_GLOBAL_REQUEST);
		packet_put_cstring("keepalive@openssh.com");
		packet_put_char(1);	 
	} else {
		channel_request_start(channel_id, "keepalive@openssh.com", 1);
	}
	packet_send();
}
