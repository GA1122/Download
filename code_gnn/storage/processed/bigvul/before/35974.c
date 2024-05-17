SMB2_ioctl(const unsigned int xid, struct cifs_tcon *tcon, u64 persistent_fid,
	   u64 volatile_fid, u32 opcode, bool is_fsctl, char *in_data,
	   u32 indatalen, char **out_data, u32 *plen  )
{
	struct smb2_ioctl_req *req;
	struct smb2_ioctl_rsp *rsp;
	struct TCP_Server_Info *server;
	struct cifs_ses *ses = tcon->ses;
	struct kvec iov[2];
	int resp_buftype;
	int num_iovecs;
	int rc = 0;

	cifs_dbg(FYI, "SMB2 IOCTL\n");

	if (out_data != NULL)
		*out_data = NULL;

	 
	if (plen)
		*plen = 0;

	if (ses && (ses->server))
		server = ses->server;
	else
		return -EIO;

	rc = small_smb2_init(SMB2_IOCTL, tcon, (void **) &req);
	if (rc)
		return rc;

	req->CtlCode = cpu_to_le32(opcode);
	req->PersistentFileId = persistent_fid;
	req->VolatileFileId = volatile_fid;

	if (indatalen) {
		req->InputCount = cpu_to_le32(indatalen);
		 
		req->InputOffset =
		       cpu_to_le32(offsetof(struct smb2_ioctl_req, Buffer) - 4);
		iov[1].iov_base = in_data;
		iov[1].iov_len = indatalen;
		num_iovecs = 2;
	} else
		num_iovecs = 1;

	req->OutputOffset = 0;
	req->OutputCount = 0;  

	 
	req->MaxOutputResponse = cpu_to_le32(0xFF00);  

	if (is_fsctl)
		req->Flags = cpu_to_le32(SMB2_0_IOCTL_IS_FSCTL);
	else
		req->Flags = 0;

	iov[0].iov_base = (char *)req;

	 

	if (indatalen) {
		iov[0].iov_len = get_rfc1002_length(req) + 4 - 1;
		inc_rfc1001_len(req, indatalen - 1);
	} else
		iov[0].iov_len = get_rfc1002_length(req) + 4;


	rc = SendReceive2(xid, ses, iov, num_iovecs, &resp_buftype, 0);
	rsp = (struct smb2_ioctl_rsp *)iov[0].iov_base;

	if ((rc != 0) && (rc != -EINVAL)) {
		if (tcon)
			cifs_stats_fail_inc(tcon, SMB2_IOCTL_HE);
		goto ioctl_exit;
	} else if (rc == -EINVAL) {
		if ((opcode != FSCTL_SRV_COPYCHUNK_WRITE) &&
		    (opcode != FSCTL_SRV_COPYCHUNK)) {
			if (tcon)
				cifs_stats_fail_inc(tcon, SMB2_IOCTL_HE);
			goto ioctl_exit;
		}
	}

	 
	if ((plen == NULL) || (out_data == NULL))
		goto ioctl_exit;

	*plen = le32_to_cpu(rsp->OutputCount);

	 
	if (*plen == 0)
		goto ioctl_exit;  
	else if (*plen > 0xFF00) {
		cifs_dbg(VFS, "srv returned invalid ioctl length: %d\n", *plen);
		*plen = 0;
		rc = -EIO;
		goto ioctl_exit;
	}

	if (get_rfc1002_length(rsp) < le32_to_cpu(rsp->OutputOffset) + *plen) {
		cifs_dbg(VFS, "Malformed ioctl resp: len %d offset %d\n", *plen,
			le32_to_cpu(rsp->OutputOffset));
		*plen = 0;
		rc = -EIO;
		goto ioctl_exit;
	}

	*out_data = kmalloc(*plen, GFP_KERNEL);
	if (*out_data == NULL) {
		rc = -ENOMEM;
		goto ioctl_exit;
	}

	memcpy(*out_data, rsp->hdr.ProtocolId + le32_to_cpu(rsp->OutputOffset),
	       *plen);
ioctl_exit:
	free_rsp_buf(resp_buftype, rsp);
	return rc;
}
