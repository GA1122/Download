session_close_by_channel(int id, void *arg)
{
	Session *s = session_by_channel(id);
	u_int i;

	if (s == NULL) {
		debug("session_close_by_channel: no session for id %d", id);
		return;
	}
	debug("session_close_by_channel: channel %d child %ld",
	    id, (long)s->pid);
	if (s->pid != 0) {
		debug("session_close_by_channel: channel %d: has child", id);
		 
		if (s->ttyfd != -1)
			session_pty_cleanup(s);
		return;
	}
	 
	channel_cancel_cleanup(s->chanid);

	 
	if (s->x11_chanids != NULL) {
		for (i = 0; s->x11_chanids[i] != -1; i++) {
			session_close_x11(s->x11_chanids[i]);
			s->x11_chanids[i] = -1;
		}
	}

	s->chanid = -1;
	session_close(s);
}