mm_answer_pty(int sock, Buffer *m)
{
	extern struct monitor *pmonitor;
	Session *s;
	int res, fd0;

	debug3("%s entering", __func__);

	buffer_clear(m);
	s = session_new();
	if (s == NULL)
		goto error;
	s->authctxt = authctxt;
	s->pw = authctxt->pw;
	s->pid = pmonitor->m_pid;
	res = pty_allocate(&s->ptyfd, &s->ttyfd, s->tty, sizeof(s->tty));
	if (res == 0)
		goto error;
	pty_setowner(authctxt->pw, s->tty);

	buffer_put_int(m, 1);
	buffer_put_cstring(m, s->tty);

	 
	if (dup2(s->ttyfd, 0) == -1)
		fatal("%s: dup2", __func__);

	mm_record_login(s, authctxt->pw);

	 
	close(0);

	 
	buffer_put_string(m, buffer_ptr(&loginmsg), buffer_len(&loginmsg));
	buffer_clear(&loginmsg);

	mm_request_send(sock, MONITOR_ANS_PTY, m);

	if (mm_send_fd(sock, s->ptyfd) == -1 ||
	    mm_send_fd(sock, s->ttyfd) == -1)
		fatal("%s: send fds failed", __func__);

	 
	if ((fd0 = open(_PATH_DEVNULL, O_RDONLY)) < 0)
		fatal("%s: open(/dev/null): %s", __func__, strerror(errno));
	if (fd0 != 0)
		error("%s: fd0 %d != 0", __func__, fd0);

	 
	close(s->ttyfd);
	s->ttyfd = s->ptyfd;
	 
	s->ptymaster = s->ptyfd;

	debug3("%s: tty %s ptyfd %d", __func__, s->tty, s->ttyfd);

	return (0);

 error:
	if (s != NULL)
		mm_session_close(s);
	buffer_put_int(m, 0);
	mm_request_send(sock, MONITOR_ANS_PTY, m);
	return (0);
}