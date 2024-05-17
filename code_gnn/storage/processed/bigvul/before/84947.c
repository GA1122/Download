smb2_lockv(const unsigned int xid, struct cifs_tcon *tcon,
	   const __u64 persist_fid, const __u64 volatile_fid, const __u32 pid,
	   const __u32 num_lock, struct smb2_lock_element *buf)
{
	int rc = 0;
	struct smb2_lock_req *req = NULL;
	struct kvec iov[2];
	struct kvec rsp_iov;
	int resp_buf_type;
	unsigned int count;
	int flags = CIFS_NO_RESP;

	cifs_dbg(FYI, "smb2_lockv num lock %d\n", num_lock);

	rc = small_smb2_init(SMB2_LOCK, tcon, (void **) &req);
	if (rc)
		return rc;

	if (encryption_required(tcon))
		flags |= CIFS_TRANSFORM_REQ;

	req->hdr.sync_hdr.ProcessId = cpu_to_le32(pid);
	req->LockCount = cpu_to_le16(num_lock);

	req->PersistentFileId = persist_fid;
	req->VolatileFileId = volatile_fid;

	count = num_lock * sizeof(struct smb2_lock_element);
	inc_rfc1001_len(req, count - sizeof(struct smb2_lock_element));

	iov[0].iov_base = (char *)req;
	 
	iov[0].iov_len = get_rfc1002_length(req) + 4 - count;
	iov[1].iov_base = (char *)buf;
	iov[1].iov_len = count;

	cifs_stats_inc(&tcon->stats.cifs_stats.num_locks);
	rc = SendReceive2(xid, tcon->ses, iov, 2, &resp_buf_type, flags,
			  &rsp_iov);
	cifs_small_buf_release(req);
	if (rc) {
		cifs_dbg(FYI, "Send error in smb2_lockv = %d\n", rc);
		cifs_stats_fail_inc(tcon, SMB2_LOCK_HE);
	}

	return rc;
}