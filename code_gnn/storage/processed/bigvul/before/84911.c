SMB2_negotiate(const unsigned int xid, struct cifs_ses *ses)
{
	struct smb2_negotiate_req *req;
	struct smb2_negotiate_rsp *rsp;
	struct kvec iov[1];
	struct kvec rsp_iov;
	int rc = 0;
	int resp_buftype;
	struct TCP_Server_Info *server = ses->server;
	int blob_offset, blob_length;
	char *security_blob;
	int flags = CIFS_NEG_OP;

	cifs_dbg(FYI, "Negotiate protocol\n");

	if (!server) {
		WARN(1, "%s: server is NULL!\n", __func__);
		return -EIO;
	}

	rc = small_smb2_init(SMB2_NEGOTIATE, NULL, (void **) &req);
	if (rc)
		return rc;

	req->hdr.sync_hdr.SessionId = 0;

	req->Dialects[0] = cpu_to_le16(ses->server->vals->protocol_id);

	req->DialectCount = cpu_to_le16(1);  
	inc_rfc1001_len(req, 2);

	 
	if (ses->sign)
		req->SecurityMode = cpu_to_le16(SMB2_NEGOTIATE_SIGNING_REQUIRED);
	else if (global_secflags & CIFSSEC_MAY_SIGN)
		req->SecurityMode = cpu_to_le16(SMB2_NEGOTIATE_SIGNING_ENABLED);
	else
		req->SecurityMode = 0;

	req->Capabilities = cpu_to_le32(ses->server->vals->req_capabilities);

	 
	if (ses->server->vals->protocol_id == SMB20_PROT_ID)
		memset(req->ClientGUID, 0, SMB2_CLIENT_GUID_SIZE);
	else {
		memcpy(req->ClientGUID, server->client_guid,
			SMB2_CLIENT_GUID_SIZE);
		if (ses->server->vals->protocol_id == SMB311_PROT_ID)
			assemble_neg_contexts(req);
	}
	iov[0].iov_base = (char *)req;
	 
	iov[0].iov_len = get_rfc1002_length(req) + 4;

	rc = SendReceive2(xid, ses, iov, 1, &resp_buftype, flags, &rsp_iov);
	cifs_small_buf_release(req);
	rsp = (struct smb2_negotiate_rsp *)rsp_iov.iov_base;
	 
	if (rc != 0)
		goto neg_exit;

	cifs_dbg(FYI, "mode 0x%x\n", rsp->SecurityMode);

	 
	if (rsp->DialectRevision == cpu_to_le16(SMB20_PROT_ID))
		cifs_dbg(FYI, "negotiated smb2.0 dialect\n");
	else if (rsp->DialectRevision == cpu_to_le16(SMB21_PROT_ID))
		cifs_dbg(FYI, "negotiated smb2.1 dialect\n");
	else if (rsp->DialectRevision == cpu_to_le16(SMB30_PROT_ID))
		cifs_dbg(FYI, "negotiated smb3.0 dialect\n");
	else if (rsp->DialectRevision == cpu_to_le16(SMB302_PROT_ID))
		cifs_dbg(FYI, "negotiated smb3.02 dialect\n");
#ifdef CONFIG_CIFS_SMB311
	else if (rsp->DialectRevision == cpu_to_le16(SMB311_PROT_ID))
		cifs_dbg(FYI, "negotiated smb3.1.1 dialect\n");
#endif  
	else {
		cifs_dbg(VFS, "Illegal dialect returned by server 0x%x\n",
			 le16_to_cpu(rsp->DialectRevision));
		rc = -EIO;
		goto neg_exit;
	}
	server->dialect = le16_to_cpu(rsp->DialectRevision);

	 
	server->negflavor = CIFS_NEGFLAVOR_EXTENDED;
	 
	server->maxBuf = min_t(unsigned int, le32_to_cpu(rsp->MaxTransactSize),
			       SMB2_MAX_BUFFER_SIZE);
	server->max_read = le32_to_cpu(rsp->MaxReadSize);
	server->max_write = le32_to_cpu(rsp->MaxWriteSize);
	 
	server->sec_mode = le16_to_cpu(rsp->SecurityMode);
	server->capabilities = le32_to_cpu(rsp->Capabilities);
	 
	server->capabilities |= SMB2_NT_FIND | SMB2_LARGE_FILES;

	security_blob = smb2_get_data_area_len(&blob_offset, &blob_length,
					       &rsp->hdr);
	 
	if (blob_length == 0)
		cifs_dbg(FYI, "missing security blob on negprot\n");

	rc = cifs_enable_signing(server, ses->sign);
	if (rc)
		goto neg_exit;
	if (blob_length) {
		rc = decode_negTokenInit(security_blob, blob_length, server);
		if (rc == 1)
			rc = 0;
		else if (rc == 0)
			rc = -EIO;
	}
neg_exit:
	free_rsp_buf(resp_buftype, rsp);
	return rc;
}
