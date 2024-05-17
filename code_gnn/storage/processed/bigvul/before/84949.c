smb2_plain_req_init(__le16 smb2_command, struct cifs_tcon *tcon,
		    void **request_buf, unsigned int *total_len)
{
	int rc;
	struct smb2_sync_hdr *shdr;

	rc = smb2_reconnect(smb2_command, tcon);
	if (rc)
		return rc;

	 
	*request_buf = cifs_small_buf_get();
	if (*request_buf == NULL) {
		 
		return -ENOMEM;
	}

	shdr = (struct smb2_sync_hdr *)(*request_buf);

	fill_small_buf(smb2_command, tcon, shdr, total_len);

	if (tcon != NULL) {
#ifdef CONFIG_CIFS_STATS2
		uint16_t com_code = le16_to_cpu(smb2_command);

		cifs_stats_inc(&tcon->stats.smb2_stats.smb2_com_sent[com_code]);
#endif
		cifs_stats_inc(&tcon->num_smbs_sent);
	}

	return rc;
}