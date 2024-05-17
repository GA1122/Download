session_pty_cleanup2(Session *s)
{
	if (s == NULL) {
		error("session_pty_cleanup: no session");
		return;
	}
	if (s->ttyfd == -1)
		return;

	debug("session_pty_cleanup: session %d release %s", s->self, s->tty);

	 
	if (s->pid != 0)
		record_logout(s->pid, s->tty, s->pw->pw_name);

	 
	if (getuid() == 0)
		pty_release(s->tty);

	 
	if (s->ptymaster != -1 && close(s->ptymaster) < 0)
		error("close(s->ptymaster/%d): %s",
		    s->ptymaster, strerror(errno));

	 
	s->ttyfd = -1;
}
