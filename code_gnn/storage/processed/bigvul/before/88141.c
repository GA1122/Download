smb2_writev_callback(struct mid_q_entry *mid)
{
	struct cifs_writedata *wdata = mid->callback_data;
	struct cifs_tcon *tcon = tlink_tcon(wdata->cfile->tlink);
	struct TCP_Server_Info *server = tcon->ses->server;
	unsigned int written;
	struct smb2_write_rsp *rsp = (struct smb2_write_rsp *)mid->resp_buf;
	struct cifs_credits credits = { .value = 0, .instance = 0 };

	switch (mid->mid_state) {
	case MID_RESPONSE_RECEIVED:
		credits.value = le16_to_cpu(rsp->sync_hdr.CreditRequest);
		credits.instance = server->reconnect_instance;
		wdata->result = smb2_check_receive(mid, server, 0);
		if (wdata->result != 0)
			break;

		written = le32_to_cpu(rsp->DataLength);
		 
		if (written > wdata->bytes)
			written &= 0xFFFF;

		if (written < wdata->bytes)
			wdata->result = -ENOSPC;
		else
			wdata->bytes = written;
		break;
	case MID_REQUEST_SUBMITTED:
	case MID_RETRY_NEEDED:
		wdata->result = -EAGAIN;
		break;
	case MID_RESPONSE_MALFORMED:
		credits.value = le16_to_cpu(rsp->sync_hdr.CreditRequest);
		credits.instance = server->reconnect_instance;
		 
	default:
		wdata->result = -EIO;
		break;
	}
#ifdef CONFIG_CIFS_SMB_DIRECT
	 
	if (wdata->mr) {
		smbd_deregister_mr(wdata->mr);
		wdata->mr = NULL;
	}
#endif
	if (wdata->result) {
		cifs_stats_fail_inc(tcon, SMB2_WRITE_HE);
		trace_smb3_write_err(0  ,
				     wdata->cfile->fid.persistent_fid,
				     tcon->tid, tcon->ses->Suid, wdata->offset,
				     wdata->bytes, wdata->result);
	} else
		trace_smb3_write_done(0  ,
				      wdata->cfile->fid.persistent_fid,
				      tcon->tid, tcon->ses->Suid,
				      wdata->offset, wdata->bytes);

	queue_work(cifsiod_wq, &wdata->work);
	DeleteMidQEntry(mid);
	add_credits(server, &credits, 0);
}