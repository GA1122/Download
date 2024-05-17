SMB2_sess_alloc_buffer(struct SMB2_sess_data *sess_data)
{
	int rc;
	struct cifs_ses *ses = sess_data->ses;
	struct smb2_sess_setup_req *req;
	struct TCP_Server_Info *server = ses->server;

	rc = small_smb2_init(SMB2_SESSION_SETUP, NULL, (void **) &req);
	if (rc)
		return rc;

	 
	req->hdr.sync_hdr.SessionId = 0;

	 
	req->PreviousSessionId = sess_data->previous_session;

	req->Flags = 0;  
	 
	req->hdr.sync_hdr.CreditRequest = cpu_to_le16(3);

	 
	if (server->sign)
		req->SecurityMode = SMB2_NEGOTIATE_SIGNING_REQUIRED;
	else if (global_secflags & CIFSSEC_MAY_SIGN)  
		req->SecurityMode = SMB2_NEGOTIATE_SIGNING_ENABLED;
	else
		req->SecurityMode = 0;

	req->Capabilities = 0;
	req->Channel = 0;  

	sess_data->iov[0].iov_base = (char *)req;
	 
	sess_data->iov[0].iov_len = get_rfc1002_length(req) + 4 - 1;
	 
	sess_data->buf0_type = CIFS_SMALL_BUFFER;

	return 0;
}
