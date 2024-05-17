sess_auth_lanman(struct sess_data *sess_data)
{
	int rc = 0;
	struct smb_hdr *smb_buf;
	SESSION_SETUP_ANDX *pSMB;
	char *bcc_ptr;
	struct cifs_ses *ses = sess_data->ses;
	char lnm_session_key[CIFS_AUTH_RESP_SIZE];
	__u32 capabilities;
	__u16 bytes_remaining;

	 
	 
	rc = sess_alloc_buffer(sess_data, 10);
	if (rc)
		goto out;

	pSMB = (SESSION_SETUP_ANDX *)sess_data->iov[0].iov_base;
	bcc_ptr = sess_data->iov[2].iov_base;
	capabilities = cifs_ssetup_hdr(ses, pSMB);

	pSMB->req.hdr.Flags2 &= ~SMBFLG2_UNICODE;

	if (ses->user_name != NULL) {
		 
		pSMB->old_req.PasswordLength = cpu_to_le16(CIFS_AUTH_RESP_SIZE);

		 
		rc = calc_lanman_hash(ses->password, ses->server->cryptkey,
				      ses->server->sec_mode & SECMODE_PW_ENCRYPT ?
				      true : false, lnm_session_key);
		if (rc)
			goto out;

		memcpy(bcc_ptr, (char *)lnm_session_key, CIFS_AUTH_RESP_SIZE);
		bcc_ptr += CIFS_AUTH_RESP_SIZE;
	} else {
		pSMB->old_req.PasswordLength = 0;
	}

	 

	cifs_dbg(FYI, "Negotiating LANMAN setting up strings\n");
	 
	ascii_ssetup_strings(&bcc_ptr, ses, sess_data->nls_cp);

	sess_data->iov[2].iov_len = (long) bcc_ptr -
			(long) sess_data->iov[2].iov_base;

	rc = sess_sendreceive(sess_data);
	if (rc)
		goto out;

	pSMB = (SESSION_SETUP_ANDX *)sess_data->iov[0].iov_base;
	smb_buf = (struct smb_hdr *)sess_data->iov[0].iov_base;

	 
	if (smb_buf->WordCount != 3) {
		rc = -EIO;
		cifs_dbg(VFS, "bad word count %d\n", smb_buf->WordCount);
		goto out;
	}

	if (le16_to_cpu(pSMB->resp.Action) & GUEST_LOGIN)
		cifs_dbg(FYI, "Guest login\n");  

	ses->Suid = smb_buf->Uid;    
	cifs_dbg(FYI, "UID = %llu\n", ses->Suid);

	bytes_remaining = get_bcc(smb_buf);
	bcc_ptr = pByteArea(smb_buf);

	 
	if (bytes_remaining == 0) {
		 
	} else if (smb_buf->Flags2 & SMBFLG2_UNICODE) {
		 
		if (((unsigned long) bcc_ptr - (unsigned long) smb_buf) % 2) {
			++bcc_ptr;
			--bytes_remaining;
		}
		decode_unicode_ssetup(&bcc_ptr, bytes_remaining, ses,
				      sess_data->nls_cp);
	} else {
		decode_ascii_ssetup(&bcc_ptr, bytes_remaining, ses,
				    sess_data->nls_cp);
	}

	rc = sess_establish_session(sess_data);
out:
	sess_data->result = rc;
	sess_data->func = NULL;
	sess_free_buffer(sess_data);
}