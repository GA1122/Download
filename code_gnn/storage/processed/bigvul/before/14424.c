session_set_fds(Session *s, int fdin, int fdout, int fderr, int ignore_fderr,
    int is_tty)
{
	if (!compat20)
		fatal("session_set_fds: called for proto != 2.0");
	 
	if (s->chanid == -1)
		fatal("no channel for session %d", s->self);
	channel_set_fds(s->chanid,
	    fdout, fdin, fderr,
	    ignore_fderr ? CHAN_EXTENDED_IGNORE : CHAN_EXTENDED_READ,
	    1, is_tty, CHAN_SES_WINDOW_DEFAULT);
}
