cifs_echo_request(struct work_struct *work)
{
	int rc;
	struct TCP_Server_Info *server = container_of(work,
					struct TCP_Server_Info, echo.work);

	 
	if (!server->ops->need_neg || server->ops->need_neg(server) ||
	    (server->ops->can_echo && !server->ops->can_echo(server)) ||
	    time_before(jiffies, server->lstrp + SMB_ECHO_INTERVAL - HZ))
		goto requeue_echo;

	rc = server->ops->echo ? server->ops->echo(server) : -ENOSYS;
	if (rc)
		cifs_dbg(FYI, "Unable to send echo request to server: %s\n",
			 server->hostname);

requeue_echo:
	queue_delayed_work(cifsiod_wq, &server->echo, SMB_ECHO_INTERVAL);
}