static int validate_t2(struct smb_t2_rsp *pSMB)
{
	unsigned int total_size;

	 
	if (pSMB->hdr.WordCount < 10)
		goto vt2_err;

	 
	if (get_unaligned_le16(&pSMB->t2_rsp.ParameterOffset) > 1024 ||
	    get_unaligned_le16(&pSMB->t2_rsp.DataOffset) > 1024)
		goto vt2_err;

	total_size = get_unaligned_le16(&pSMB->t2_rsp.ParameterCount);
	if (total_size >= 512)
		goto vt2_err;

	 
	total_size += get_unaligned_le16(&pSMB->t2_rsp.DataCount);
	if (total_size > get_bcc(&pSMB->hdr) ||
	    total_size >= CIFSMaxBufSize + MAX_CIFS_HDR_SIZE)
		goto vt2_err;

	return 0;
vt2_err:
	cifs_dump_mem("Invalid transact2 SMB: ", (char *)pSMB,
		sizeof(struct smb_t2_rsp) + 16);
	return -EINVAL;
}