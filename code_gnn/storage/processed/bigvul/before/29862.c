server_unresponsive(struct TCP_Server_Info *server)
{
	 
	if (server->tcpStatus == CifsGood &&
	    time_after(jiffies, server->lstrp + 2 * SMB_ECHO_INTERVAL)) {
		cifs_dbg(VFS, "Server %s has not responded in %d seconds. Reconnecting...\n",
			 server->hostname, (2 * SMB_ECHO_INTERVAL) / HZ);
		cifs_reconnect(server);
		wake_up(&server->response_q);
		return true;
	}

	return false;
}
