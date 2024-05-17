privsep_postauth(Authctxt *authctxt)
{
	if (authctxt->pw->pw_uid == 0) {
		 
		use_privsep = 0;
		goto skip;
	}

	 
	monitor_reinit(pmonitor);

	pmonitor->m_pid = fork();
	if (pmonitor->m_pid == -1)
		fatal("fork of unprivileged child failed");
	else if (pmonitor->m_pid != 0) {
		verbose("User child is on pid %ld", (long)pmonitor->m_pid);
		buffer_clear(&loginmsg);
		monitor_child_postauth(pmonitor);

		 
		exit(0);
	}

	 

	close(pmonitor->m_sendfd);
	pmonitor->m_sendfd = -1;

	 
	demote_sensitive_data();

	 
	do_setusercontext(authctxt->pw);

 skip:
	 
	monitor_apply_keystate(pmonitor);

	 
	packet_set_authenticated();
}
