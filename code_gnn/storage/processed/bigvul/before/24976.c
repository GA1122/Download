CIFSSMBQFileInfo(const int xid, struct cifs_tcon *tcon,
		 u16 netfid, FILE_ALL_INFO *pFindData)
{
	struct smb_t2_qfi_req *pSMB = NULL;
	struct smb_t2_qfi_rsp *pSMBr = NULL;
	int rc = 0;
	int bytes_returned;
	__u16 params, byte_count;

QFileInfoRetry:
	rc = smb_init(SMB_COM_TRANSACTION2, 15, tcon, (void **) &pSMB,
		      (void **) &pSMBr);
	if (rc)
		return rc;

	params = 2   + 2  ;
	pSMB->t2.TotalDataCount = 0;
	pSMB->t2.MaxParameterCount = cpu_to_le16(4);
	 
	pSMB->t2.MaxDataCount = cpu_to_le16(CIFSMaxBufSize);
	pSMB->t2.MaxSetupCount = 0;
	pSMB->t2.Reserved = 0;
	pSMB->t2.Flags = 0;
	pSMB->t2.Timeout = 0;
	pSMB->t2.Reserved2 = 0;
	pSMB->t2.ParameterOffset = cpu_to_le16(offsetof(struct smb_t2_qfi_req,
					       Fid) - 4);
	pSMB->t2.DataCount = 0;
	pSMB->t2.DataOffset = 0;
	pSMB->t2.SetupCount = 1;
	pSMB->t2.Reserved3 = 0;
	pSMB->t2.SubCommand = cpu_to_le16(TRANS2_QUERY_FILE_INFORMATION);
	byte_count = params + 1   ;
	pSMB->t2.TotalParameterCount = cpu_to_le16(params);
	pSMB->t2.ParameterCount = pSMB->t2.TotalParameterCount;
	pSMB->InformationLevel = cpu_to_le16(SMB_QUERY_FILE_ALL_INFO);
	pSMB->Pad = 0;
	pSMB->Fid = netfid;
	inc_rfc1001_len(pSMB, byte_count);

	rc = SendReceive(xid, tcon->ses, (struct smb_hdr *) pSMB,
			 (struct smb_hdr *) pSMBr, &bytes_returned, 0);
	if (rc) {
		cFYI(1, "Send error in QPathInfo = %d", rc);
	} else {		 
		rc = validate_t2((struct smb_t2_rsp *)pSMBr);

		if (rc)  
			rc = -EIO;
		else if (get_bcc(&pSMBr->hdr) < 40)
			rc = -EIO;	 
		else if (pFindData) {
			__u16 data_offset = le16_to_cpu(pSMBr->t2.DataOffset);
			memcpy((char *) pFindData,
			       (char *) &pSMBr->hdr.Protocol +
			       data_offset, sizeof(FILE_ALL_INFO));
		} else
		    rc = -ENOMEM;
	}
	cifs_buf_release(pSMB);
	if (rc == -EAGAIN)
		goto QFileInfoRetry;

	return rc;
}