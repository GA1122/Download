int smb3_validate_negotiate(const unsigned int xid, struct cifs_tcon *tcon)
{
	int rc = 0;
	struct validate_negotiate_info_req vneg_inbuf;
	struct validate_negotiate_info_rsp *pneg_rsp;
	u32 rsplen;

	cifs_dbg(FYI, "validate negotiate\n");

	 
	if (tcon->ses->server->sign == false)
		return 0;  

	vneg_inbuf.Capabilities =
			cpu_to_le32(tcon->ses->server->vals->req_capabilities);
	memcpy(vneg_inbuf.Guid, tcon->ses->server->client_guid,
					SMB2_CLIENT_GUID_SIZE);

	if (tcon->ses->sign)
		vneg_inbuf.SecurityMode =
			cpu_to_le16(SMB2_NEGOTIATE_SIGNING_REQUIRED);
	else if (global_secflags & CIFSSEC_MAY_SIGN)
		vneg_inbuf.SecurityMode =
			cpu_to_le16(SMB2_NEGOTIATE_SIGNING_ENABLED);
	else
		vneg_inbuf.SecurityMode = 0;

	vneg_inbuf.DialectCount = cpu_to_le16(1);
	vneg_inbuf.Dialects[0] =
		cpu_to_le16(tcon->ses->server->vals->protocol_id);

	rc = SMB2_ioctl(xid, tcon, NO_FILE_ID, NO_FILE_ID,
		FSCTL_VALIDATE_NEGOTIATE_INFO, true  ,
		(char *)&vneg_inbuf, sizeof(struct validate_negotiate_info_req),
		(char **)&pneg_rsp, &rsplen);

	if (rc != 0) {
		cifs_dbg(VFS, "validate protocol negotiate failed: %d\n", rc);
		return -EIO;
	}

	if (rsplen != sizeof(struct validate_negotiate_info_rsp)) {
		cifs_dbg(VFS, "invalid size of protocol negotiate response\n");
		return -EIO;
	}

	 
	if (pneg_rsp->Dialect !=
			cpu_to_le16(tcon->ses->server->vals->protocol_id))
		goto vneg_out;

	if (pneg_rsp->SecurityMode != cpu_to_le16(tcon->ses->server->sec_mode))
		goto vneg_out;

	 

	if ((le32_to_cpu(pneg_rsp->Capabilities) | SMB2_NT_FIND |
	      SMB2_LARGE_FILES) != tcon->ses->server->capabilities)
		goto vneg_out;

	 
	cifs_dbg(FYI, "validate negotiate info successful\n");
	return 0;

vneg_out:
	cifs_dbg(VFS, "protocol revalidation - security settings mismatch\n");
	return -EIO;
}
