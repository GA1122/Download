session_exit_message(Session *s, int status)
{
	Channel *c;

	if ((c = channel_lookup(s->chanid)) == NULL)
		fatal("session_exit_message: session %d: no channel %d",
		    s->self, s->chanid);
	debug("session_exit_message: session %d channel %d pid %ld",
	    s->self, s->chanid, (long)s->pid);

	if (WIFEXITED(status)) {
		channel_request_start(s->chanid, "exit-status", 0);
		packet_put_int(WEXITSTATUS(status));
		packet_send();
	} else if (WIFSIGNALED(status)) {
		channel_request_start(s->chanid, "exit-signal", 0);
		packet_put_cstring(sig2name(WTERMSIG(status)));
#ifdef WCOREDUMP
		packet_put_char(WCOREDUMP(status)? 1 : 0);
#else  
		packet_put_char(0);
#endif  
		packet_put_cstring("");
		packet_put_cstring("");
		packet_send();
	} else {
		 
		packet_disconnect("wait returned status %04x.", status);
	}

	 
	debug("session_exit_message: release channel %d", s->chanid);

	 
	channel_register_cleanup(c->self, session_close_by_channel, 1);

	 
	if (c->ostate != CHAN_OUTPUT_CLOSED)
		chan_write_failed(c);
}