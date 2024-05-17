server_request_session(void)
{
	Channel *c;

	debug("input_session_request");
	packet_check_eom();

	if (no_more_sessions) {
		packet_disconnect("Possible attack: attempt to open a session "
		    "after additional sessions disabled");
	}

	 
	c = channel_new("session", SSH_CHANNEL_LARVAL,
	    -1, -1, -1,  0, CHAN_SES_PACKET_DEFAULT,
	    0, "server-session", 1);
	if (session_open(the_authctxt, c->self) != 1) {
		debug("session open failed, free channel %d", c->self);
		channel_free(c);
		return NULL;
	}
	channel_register_cleanup(c->self, session_close_by_channel, 0);
	return c;
}
