SMB2_tdis(const unsigned int xid, struct cifs_tcon *tcon)
{
	struct smb2_tree_disconnect_req *req;  
	int rc = 0;
	struct TCP_Server_Info *server;
	struct cifs_ses *ses = tcon->ses;
	int flags = 0;

	cifs_dbg(FYI, "Tree Disconnect\n");

	if (ses && (ses->server))
		server = ses->server;
	else
		return -EIO;

	if ((tcon->need_reconnect) || (tcon->ses->need_reconnect))
		return 0;

	rc = small_smb2_init(SMB2_TREE_DISCONNECT, tcon, (void **) &req);
	if (rc)
		return rc;

	if (encryption_required(tcon))
		flags |= CIFS_TRANSFORM_REQ;

	rc = SendReceiveNoRsp(xid, ses, (char *)req, flags);
	cifs_small_buf_release(req);
	if (rc)
		cifs_stats_fail_inc(tcon, SMB2_TREE_DISCONNECT_HE);

	return rc;
}