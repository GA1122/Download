SMB2_logoff(const unsigned int xid, struct cifs_ses *ses)
{
	struct smb2_logoff_req *req;  
	int rc = 0;
	struct TCP_Server_Info *server;
	int flags = 0;

	cifs_dbg(FYI, "disconnect session %p\n", ses);

	if (ses && (ses->server))
		server = ses->server;
	else
		return -EIO;

	 
	if (ses->need_reconnect)
		goto smb2_session_already_dead;

	rc = small_smb2_init(SMB2_LOGOFF, NULL, (void **) &req);
	if (rc)
		return rc;

	  
	req->hdr.sync_hdr.SessionId = ses->Suid;

	if (ses->session_flags & SMB2_SESSION_FLAG_ENCRYPT_DATA)
		flags |= CIFS_TRANSFORM_REQ;
	else if (server->sign)
		req->hdr.sync_hdr.Flags |= SMB2_FLAGS_SIGNED;

	rc = SendReceiveNoRsp(xid, ses, (char *) req, flags);
	cifs_small_buf_release(req);
	 

smb2_session_already_dead:
	return rc;
}
