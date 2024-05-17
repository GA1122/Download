SMB2_read(const unsigned int xid, struct cifs_io_parms *io_parms,
	  unsigned int *nbytes, char **buf, int *buf_type)
{
	struct smb_rqst rqst;
	int resp_buftype, rc = -EACCES;
	struct smb2_read_plain_req *req = NULL;
	struct smb2_read_rsp *rsp = NULL;
	struct kvec iov[1];
	struct kvec rsp_iov;
	unsigned int total_len;
	int flags = CIFS_LOG_ERROR;
	struct cifs_ses *ses = io_parms->tcon->ses;

	*nbytes = 0;
	rc = smb2_new_read_req((void **)&req, &total_len, io_parms, NULL, 0, 0);
	if (rc)
		return rc;

	if (smb3_encryption_required(io_parms->tcon))
		flags |= CIFS_TRANSFORM_REQ;

	iov[0].iov_base = (char *)req;
	iov[0].iov_len = total_len;

	memset(&rqst, 0, sizeof(struct smb_rqst));
	rqst.rq_iov = iov;
	rqst.rq_nvec = 1;

	rc = cifs_send_recv(xid, ses, &rqst, &resp_buftype, flags, &rsp_iov);
	cifs_small_buf_release(req);

	rsp = (struct smb2_read_rsp *)rsp_iov.iov_base;

	if (rc) {
		if (rc != -ENODATA) {
			cifs_stats_fail_inc(io_parms->tcon, SMB2_READ_HE);
			cifs_dbg(VFS, "Send error in read = %d\n", rc);
			trace_smb3_read_err(xid, req->PersistentFileId,
					    io_parms->tcon->tid, ses->Suid,
					    io_parms->offset, io_parms->length,
					    rc);
		} else
			trace_smb3_read_done(xid, req->PersistentFileId,
				    io_parms->tcon->tid, ses->Suid,
				    io_parms->offset, 0);
		free_rsp_buf(resp_buftype, rsp_iov.iov_base);
		return rc == -ENODATA ? 0 : rc;
	} else
		trace_smb3_read_done(xid, req->PersistentFileId,
				    io_parms->tcon->tid, ses->Suid,
				    io_parms->offset, io_parms->length);

	*nbytes = le32_to_cpu(rsp->DataLength);
	if ((*nbytes > CIFS_MAX_MSGSIZE) ||
	    (*nbytes > io_parms->length)) {
		cifs_dbg(FYI, "bad length %d for count %d\n",
			 *nbytes, io_parms->length);
		rc = -EIO;
		*nbytes = 0;
	}

	if (*buf) {
		memcpy(*buf, (char *)rsp + rsp->DataOffset, *nbytes);
		free_rsp_buf(resp_buftype, rsp_iov.iov_base);
	} else if (resp_buftype != CIFS_NO_BUFFER) {
		*buf = rsp_iov.iov_base;
		if (resp_buftype == CIFS_SMALL_BUFFER)
			*buf_type = CIFS_SMALL_BUFFER;
		else if (resp_buftype == CIFS_LARGE_BUFFER)
			*buf_type = CIFS_LARGE_BUFFER;
	}
	return rc;
}