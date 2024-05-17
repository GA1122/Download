SMB2_logoff(const unsigned int xid, struct cifs_ses *ses)
{
	struct smb2_logoff_req *req;  
	int rc = 0;
	struct TCP_Server_Info *server;

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

	  
	req->hdr.SessionId = ses->Suid;
	if (server->sign)
		req->hdr.Flags |= SMB2_FLAGS_SIGNED;

	rc = SendReceiveNoRsp(xid, ses, (char *) &req->hdr, 0);
	 

smb2_session_already_dead:
	return rc;
}
