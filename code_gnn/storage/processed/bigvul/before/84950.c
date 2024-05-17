smb2_readv_callback(struct mid_q_entry *mid)
{
	struct cifs_readdata *rdata = mid->callback_data;
	struct cifs_tcon *tcon = tlink_tcon(rdata->cfile->tlink);
	struct TCP_Server_Info *server = tcon->ses->server;
	struct smb2_sync_hdr *shdr =
				(struct smb2_sync_hdr *)rdata->iov[1].iov_base;
	unsigned int credits_received = 1;
	struct smb_rqst rqst = { .rq_iov = rdata->iov,
				 .rq_nvec = 2,
				 .rq_pages = rdata->pages,
				 .rq_npages = rdata->nr_pages,
				 .rq_pagesz = rdata->pagesz,
				 .rq_tailsz = rdata->tailsz };

	cifs_dbg(FYI, "%s: mid=%llu state=%d result=%d bytes=%u\n",
		 __func__, mid->mid, mid->mid_state, rdata->result,
		 rdata->bytes);

	switch (mid->mid_state) {
	case MID_RESPONSE_RECEIVED:
		credits_received = le16_to_cpu(shdr->CreditRequest);
		 
		if (server->sign) {
			int rc;

			rc = smb2_verify_signature(&rqst, server);
			if (rc)
				cifs_dbg(VFS, "SMB signature verification returned error = %d\n",
					 rc);
		}
		 
		task_io_account_read(rdata->got_bytes);
		cifs_stats_bytes_read(tcon, rdata->got_bytes);
		break;
	case MID_REQUEST_SUBMITTED:
	case MID_RETRY_NEEDED:
		rdata->result = -EAGAIN;
		if (server->sign && rdata->got_bytes)
			 
			rdata->got_bytes = 0;
		 
		task_io_account_read(rdata->got_bytes);
		cifs_stats_bytes_read(tcon, rdata->got_bytes);
		break;
	default:
		if (rdata->result != -ENODATA)
			rdata->result = -EIO;
	}

	if (rdata->result)
		cifs_stats_fail_inc(tcon, SMB2_READ_HE);

	queue_work(cifsiod_wq, &rdata->work);
	mutex_lock(&server->srv_mutex);
	DeleteMidQEntry(mid);
	mutex_unlock(&server->srv_mutex);
	add_credits(server, credits_received, 0);
}