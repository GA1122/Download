smb2_new_read_req(void **buf, unsigned int *total_len,
		  struct cifs_io_parms *io_parms, unsigned int remaining_bytes,
		  int request_type)
{
	int rc = -EACCES;
	struct smb2_read_plain_req *req = NULL;
	struct smb2_sync_hdr *shdr;

	rc = smb2_plain_req_init(SMB2_READ, io_parms->tcon, (void **) &req,
				 total_len);
	if (rc)
		return rc;
	if (io_parms->tcon->ses->server == NULL)
		return -ECONNABORTED;

	shdr = &req->sync_hdr;
	shdr->ProcessId = cpu_to_le32(io_parms->pid);

	req->PersistentFileId = io_parms->persistent_fid;
	req->VolatileFileId = io_parms->volatile_fid;
	req->ReadChannelInfoOffset = 0;  
	req->ReadChannelInfoLength = 0;  
	req->Channel = 0;  
	req->MinimumCount = 0;
	req->Length = cpu_to_le32(io_parms->length);
	req->Offset = cpu_to_le64(io_parms->offset);

	if (request_type & CHAINED_REQUEST) {
		if (!(request_type & END_OF_CHAIN)) {
			 
			*total_len = DIV_ROUND_UP(*total_len, 8) * 8;
			shdr->NextCommand = cpu_to_le32(*total_len);
		} else  
			shdr->NextCommand = 0;
		if (request_type & RELATED_REQUEST) {
			shdr->Flags |= SMB2_FLAGS_RELATED_OPERATIONS;
			 
			shdr->SessionId = 0xFFFFFFFF;
			shdr->TreeId = 0xFFFFFFFF;
			req->PersistentFileId = 0xFFFFFFFF;
			req->VolatileFileId = 0xFFFFFFFF;
		}
	}
	if (remaining_bytes > io_parms->length)
		req->RemainingBytes = cpu_to_le32(remaining_bytes);
	else
		req->RemainingBytes = 0;

	*buf = req;
	return rc;
}