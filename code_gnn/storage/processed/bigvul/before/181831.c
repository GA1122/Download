 privsep_preauth(Authctxt *authctxt)
 {
 	int status, r;
 	pid_t pid;
 	struct ssh_sandbox *box = NULL;
 
 	 
 	pmonitor = monitor_init();
 	 
 	pmonitor->m_pkex = &active_state->kex;
 
 	if (use_privsep == PRIVSEP_ON)
 		box = ssh_sandbox_init();
 	pid = fork();
 	if (pid == -1) {
 		fatal("fork of unprivileged child failed");
 	} else if (pid != 0) {
 		debug2("Network child is on pid %ld", (long)pid);
 
 		pmonitor->m_pid = pid;
 		if (have_agent) {
 			r = ssh_get_authentication_socket(&auth_sock);
 			if (r != 0) {
 				error("Could not get agent socket: %s",
 				    ssh_err(r));
 				have_agent = 0;
 			}
 		}
 		if (box != NULL)
  			ssh_sandbox_parent_preauth(box, pid);
  		monitor_child_preauth(authctxt, pmonitor);
  
		 
		monitor_sync(pmonitor);
  		 
  		while (waitpid(pid, &status, 0) < 0) {
  			if (errno == EINTR)
 				continue;
 			pmonitor->m_pid = -1;
 			fatal("%s: waitpid: %s", __func__, strerror(errno));
 		}
 		privsep_is_preauth = 0;
 		pmonitor->m_pid = -1;
 		if (WIFEXITED(status)) {
 			if (WEXITSTATUS(status) != 0)
 				fatal("%s: preauth child exited with status %d",
 				    __func__, WEXITSTATUS(status));
 		} else if (WIFSIGNALED(status))
 			fatal("%s: preauth child terminated by signal %d",
 			    __func__, WTERMSIG(status));
 		if (box != NULL)
 			ssh_sandbox_parent_finish(box);
 		return 1;
 	} else {
 		 
 		close(pmonitor->m_sendfd);
 		close(pmonitor->m_log_recvfd);
 
 		 
 		set_log_handler(mm_log_handler, pmonitor);
 
 		privsep_preauth_child();
 		setproctitle("%s", "[net]");
 		if (box != NULL)
 			ssh_sandbox_child(box);
 
 		return 0;
 	}
 }