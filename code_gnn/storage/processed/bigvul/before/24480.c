cifs_echo_request(struct work_struct *work)
{
	int rc;
	struct TCP_Server_Info *server = container_of(work,
					struct TCP_Server_Info, echo.work);

	 
	if (server->maxBuf == 0 ||
	    time_before(jiffies, server->lstrp + SMB_ECHO_INTERVAL - HZ))
		goto requeue_echo;

	rc = CIFSSMBEcho(server);
	if (rc)
		cFYI(1, "Unable to send echo request to server: %s",
			server->hostname);

requeue_echo:
	queue_delayed_work(system_nrt_wq, &server->echo, SMB_ECHO_INTERVAL);
}
