char *enl_send_and_wait(char *msg)
{
	char *reply = IPC_TIMEOUT;
	sighandler_t old_alrm;

	 
	if (e17_fake_ipc)
		return IPC_FAKE;

	if (ipc_win == None) {
		 
		 
		for (; enl_ipc_get_win() == None;) {
			if (e17_fake_ipc)
				return IPC_FAKE;
			else
				sleep(1);
		}
	}
	old_alrm = (sighandler_t) signal(SIGALRM, (sighandler_t) enl_ipc_timeout);
	for (; reply == IPC_TIMEOUT;) {
		timeout = 0;
		enl_ipc_send(msg);
		for (; !(reply = enl_ipc_get(enl_wait_for_reply())););
		if (reply == IPC_TIMEOUT) {
			 
			D(("IPC timed out.  IPC window has gone. Clearing ipc_win.\n"));
			XSelectInput(disp, ipc_win, None);
			ipc_win = None;
		}
	}
	signal(SIGALRM, old_alrm);
	return(reply);
}
