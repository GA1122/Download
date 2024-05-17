SMB2_sess_setup(const unsigned int xid, struct cifs_ses *ses,
		const struct nls_table *nls_cp)
{
	struct smb2_sess_setup_req *req;
	struct smb2_sess_setup_rsp *rsp = NULL;
	struct kvec iov[2];
	int rc = 0;
	int resp_buftype;
	__le32 phase = NtLmNegotiate;  
	struct TCP_Server_Info *server = ses->server;
	u16 blob_length = 0;
	char *security_blob;
	char *ntlmssp_blob = NULL;
	bool use_spnego = false;  

	cifs_dbg(FYI, "Session Setup\n");

	if (!server) {
		WARN(1, "%s: server is NULL!\n", __func__);
		return -EIO;
	}

	 
	kfree(ses->auth_key.response);
	ses->auth_key.response = NULL;

	 
	ses->ntlmssp = kmalloc(sizeof(struct ntlmssp_auth), GFP_KERNEL);
	if (!ses->ntlmssp)
		return -ENOMEM;
	ses->ntlmssp->sesskey_per_smbsess = true;

	 
	ses->sectype = RawNTLMSSP;

ssetup_ntlmssp_authenticate:
	if (phase == NtLmChallenge)
		phase = NtLmAuthenticate;  

	rc = small_smb2_init(SMB2_SESSION_SETUP, NULL, (void **) &req);
	if (rc)
		return rc;

	req->hdr.SessionId = 0;  
	req->VcNumber = 0;  
	 
	req->hdr.CreditRequest = cpu_to_le16(3);

	 
	if (server->sign)
		req->SecurityMode = SMB2_NEGOTIATE_SIGNING_REQUIRED;
	else if (global_secflags & CIFSSEC_MAY_SIGN)  
		req->SecurityMode = SMB2_NEGOTIATE_SIGNING_ENABLED;
	else
		req->SecurityMode = 0;

	req->Capabilities = 0;
	req->Channel = 0;  

	iov[0].iov_base = (char *)req;
	 
	iov[0].iov_len = get_rfc1002_length(req) + 4 - 1;
	if (phase == NtLmNegotiate) {
		ntlmssp_blob = kmalloc(sizeof(struct _NEGOTIATE_MESSAGE),
				       GFP_KERNEL);
		if (ntlmssp_blob == NULL) {
			rc = -ENOMEM;
			goto ssetup_exit;
		}
		build_ntlmssp_negotiate_blob(ntlmssp_blob, ses);
		if (use_spnego) {
			 
			 
			cifs_dbg(VFS, "spnego not supported for SMB2 yet\n");
			rc = -EOPNOTSUPP;
			kfree(ntlmssp_blob);
			goto ssetup_exit;
		} else {
			blob_length = sizeof(struct _NEGOTIATE_MESSAGE);
			 
			security_blob = ntlmssp_blob;
		}
	} else if (phase == NtLmAuthenticate) {
		req->hdr.SessionId = ses->Suid;
		ntlmssp_blob = kzalloc(sizeof(struct _NEGOTIATE_MESSAGE) + 500,
				       GFP_KERNEL);
		if (ntlmssp_blob == NULL) {
			rc = -ENOMEM;
			goto ssetup_exit;
		}
		rc = build_ntlmssp_auth_blob(ntlmssp_blob, &blob_length, ses,
					     nls_cp);
		if (rc) {
			cifs_dbg(FYI, "build_ntlmssp_auth_blob failed %d\n",
				 rc);
			goto ssetup_exit;  
		}
		if (use_spnego) {
			 
			cifs_dbg(VFS, "spnego not supported for SMB2 yet\n");
			rc = -EOPNOTSUPP;
			kfree(ntlmssp_blob);
			goto ssetup_exit;
		} else {
			security_blob = ntlmssp_blob;
		}
	} else {
		cifs_dbg(VFS, "illegal ntlmssp phase\n");
		rc = -EIO;
		goto ssetup_exit;
	}

	 
	req->SecurityBufferOffset =
				cpu_to_le16(sizeof(struct smb2_sess_setup_req) -
					    1   - 4  );
	req->SecurityBufferLength = cpu_to_le16(blob_length);
	iov[1].iov_base = security_blob;
	iov[1].iov_len = blob_length;

	inc_rfc1001_len(req, blob_length - 1  );

	 

	rc = SendReceive2(xid, ses, iov, 2, &resp_buftype,
			  CIFS_LOG_ERROR | CIFS_NEG_OP);

	kfree(security_blob);
	rsp = (struct smb2_sess_setup_rsp *)iov[0].iov_base;
	if (resp_buftype != CIFS_NO_BUFFER &&
	    rsp->hdr.Status == STATUS_MORE_PROCESSING_REQUIRED) {
		if (phase != NtLmNegotiate) {
			cifs_dbg(VFS, "Unexpected more processing error\n");
			goto ssetup_exit;
		}
		if (offsetof(struct smb2_sess_setup_rsp, Buffer) - 4 !=
				le16_to_cpu(rsp->SecurityBufferOffset)) {
			cifs_dbg(VFS, "Invalid security buffer offset %d\n",
				 le16_to_cpu(rsp->SecurityBufferOffset));
			rc = -EIO;
			goto ssetup_exit;
		}

		 
		phase = NtLmChallenge;  
		rc = 0;  
		ses->Suid = rsp->hdr.SessionId;
		rc = decode_ntlmssp_challenge(rsp->Buffer,
				le16_to_cpu(rsp->SecurityBufferLength), ses);
	}

	 
	 
	if (rc != 0)
		goto ssetup_exit;

	ses->session_flags = le16_to_cpu(rsp->SessionFlags);
	if (ses->session_flags & SMB2_SESSION_FLAG_ENCRYPT_DATA)
		cifs_dbg(VFS, "SMB3 encryption not supported yet\n");
ssetup_exit:
	free_rsp_buf(resp_buftype, rsp);

	 
	if ((phase == NtLmChallenge) && (rc == 0))
		goto ssetup_ntlmssp_authenticate;

	if (!rc) {
		mutex_lock(&server->srv_mutex);
		if (server->sign && server->ops->generate_signingkey) {
			rc = server->ops->generate_signingkey(ses);
			kfree(ses->auth_key.response);
			ses->auth_key.response = NULL;
			if (rc) {
				cifs_dbg(FYI,
					"SMB3 session key generation failed\n");
				mutex_unlock(&server->srv_mutex);
				goto keygen_exit;
			}
		}
		if (!server->session_estab) {
			server->sequence_number = 0x2;
			server->session_estab = true;
		}
		mutex_unlock(&server->srv_mutex);

		cifs_dbg(FYI, "SMB2/3 session established successfully\n");
		spin_lock(&GlobalMid_Lock);
		ses->status = CifsGood;
		ses->need_reconnect = false;
		spin_unlock(&GlobalMid_Lock);
	}

keygen_exit:
	if (!server->sign) {
		kfree(ses->auth_key.response);
		ses->auth_key.response = NULL;
	}
	kfree(ses->ntlmssp);

	return rc;
}
