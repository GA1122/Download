smb2_reconnect(__le16 smb2_command, struct cifs_tcon *tcon)
{
	int rc;
	struct nls_table *nls_codepage;
	struct cifs_ses *ses;
	struct TCP_Server_Info *server;
	int retries;

	 
	if (tcon == NULL)
		return 0;

	if (smb2_command == SMB2_TREE_CONNECT)
		return 0;

	if (tcon->tidStatus == CifsExiting) {
		 
		if ((smb2_command != SMB2_WRITE) &&
		   (smb2_command != SMB2_CREATE) &&
		   (smb2_command != SMB2_TREE_DISCONNECT)) {
			cifs_dbg(FYI, "can not send cmd %d while umounting\n",
				 smb2_command);
			return -ENODEV;
		}
	}
	if ((!tcon->ses) || (tcon->ses->status == CifsExiting) ||
	    (!tcon->ses->server))
		return -EIO;

	ses = tcon->ses;
	server = ses->server;

	retries = server->nr_targets;

	 
	while (server->tcpStatus == CifsNeedReconnect) {
		 
		switch (smb2_command) {
		 
		case SMB2_TREE_DISCONNECT:
		case SMB2_CANCEL:
		case SMB2_CLOSE:
		case SMB2_OPLOCK_BREAK:
			return -EAGAIN;
		}

		rc = wait_event_interruptible_timeout(server->response_q,
						      (server->tcpStatus != CifsNeedReconnect),
						      10 * HZ);
		if (rc < 0) {
			cifs_dbg(FYI, "%s: aborting reconnect due to a received"
				 " signal by the process\n", __func__);
			return -ERESTARTSYS;
		}

		 
		if (server->tcpStatus != CifsNeedReconnect)
			break;

		if (--retries)
			continue;

		 
		if (!tcon->retry) {
			cifs_dbg(FYI, "gave up waiting on reconnect in smb_init\n");
			return -EHOSTDOWN;
		}
		retries = server->nr_targets;
	}

	if (!tcon->ses->need_reconnect && !tcon->need_reconnect)
		return 0;

	nls_codepage = load_nls_default();

	 
	mutex_lock(&tcon->ses->session_mutex);

	 
	if (server->tcpStatus == CifsNeedReconnect) {
		rc = -EHOSTDOWN;
		mutex_unlock(&tcon->ses->session_mutex);
		goto out;
	}

	rc = cifs_negotiate_protocol(0, tcon->ses);
	if (!rc && tcon->ses->need_reconnect)
		rc = cifs_setup_session(0, tcon->ses, nls_codepage);

	if (rc || !tcon->need_reconnect) {
		mutex_unlock(&tcon->ses->session_mutex);
		goto out;
	}

	cifs_mark_open_files_invalid(tcon);
	if (tcon->use_persistent)
		tcon->need_reopen_files = true;

	rc = __smb2_reconnect(nls_codepage, tcon);
	mutex_unlock(&tcon->ses->session_mutex);

	cifs_dbg(FYI, "reconnect tcon rc = %d\n", rc);
	if (rc) {
		 
		printk_once(KERN_WARNING "reconnect tcon failed rc = %d\n", rc);
		goto out;
	}

	if (smb2_command != SMB2_INTERNAL_CMD)
		queue_delayed_work(cifsiod_wq, &server->reconnect, 0);

	atomic_inc(&tconInfoReconnectCount);
out:
	 
	 
	switch (smb2_command) {
	case SMB2_FLUSH:
	case SMB2_READ:
	case SMB2_WRITE:
	case SMB2_LOCK:
	case SMB2_IOCTL:
	case SMB2_QUERY_DIRECTORY:
	case SMB2_CHANGE_NOTIFY:
	case SMB2_QUERY_INFO:
	case SMB2_SET_INFO:
		rc = -EAGAIN;
	}
	unload_nls(nls_codepage);
	return rc;
}
