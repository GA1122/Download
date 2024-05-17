SMB2_ioctl_init(struct cifs_tcon *tcon, struct smb_rqst *rqst,
		u64 persistent_fid, u64 volatile_fid, u32 opcode,
		bool is_fsctl, char *in_data, u32 indatalen,
		__u32 max_response_size)
{
	struct smb2_ioctl_req *req;
	struct kvec *iov = rqst->rq_iov;
	unsigned int total_len;
	int rc;

	rc = smb2_plain_req_init(SMB2_IOCTL, tcon, (void **) &req, &total_len);
	if (rc)
		return rc;

	req->CtlCode = cpu_to_le32(opcode);
	req->PersistentFileId = persistent_fid;
	req->VolatileFileId = volatile_fid;

	iov[0].iov_base = (char *)req;
	 
	if (indatalen) {
		req->InputCount = cpu_to_le32(indatalen);
		 
		req->InputOffset =
		       cpu_to_le32(offsetof(struct smb2_ioctl_req, Buffer));
		rqst->rq_nvec = 2;
		iov[0].iov_len = total_len - 1;
		iov[1].iov_base = in_data;
		iov[1].iov_len = indatalen;
	} else {
		rqst->rq_nvec = 1;
		iov[0].iov_len = total_len;
	}

	req->OutputOffset = 0;
	req->OutputCount = 0;  

	 
	req->MaxOutputResponse = cpu_to_le32(max_response_size);

	if (is_fsctl)
		req->Flags = cpu_to_le32(SMB2_0_IOCTL_IS_FSCTL);
	else
		req->Flags = 0;

	 
	if (opcode == FSCTL_VALIDATE_NEGOTIATE_INFO)
		req->sync_hdr.Flags |= SMB2_FLAGS_SIGNED;

	return 0;
}
