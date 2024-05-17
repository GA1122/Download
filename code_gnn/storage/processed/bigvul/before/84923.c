SMB2_sess_sendreceive(struct SMB2_sess_data *sess_data)
{
	int rc;
	struct smb2_sess_setup_req *req = sess_data->iov[0].iov_base;
	struct kvec rsp_iov = { NULL, 0 };

	 
	req->SecurityBufferOffset =
		cpu_to_le16(sizeof(struct smb2_sess_setup_req) -
			1   - 4  );
	req->SecurityBufferLength = cpu_to_le16(sess_data->iov[1].iov_len);

	inc_rfc1001_len(req, sess_data->iov[1].iov_len - 1  );

	 

	rc = SendReceive2(sess_data->xid, sess_data->ses,
				sess_data->iov, 2,
				&sess_data->buf0_type,
				CIFS_LOG_ERROR | CIFS_NEG_OP, &rsp_iov);
	cifs_small_buf_release(sess_data->iov[0].iov_base);
	memcpy(&sess_data->iov[0], &rsp_iov, sizeof(struct kvec));

	return rc;
}