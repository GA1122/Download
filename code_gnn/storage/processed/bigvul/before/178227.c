 void chain_reply(struct smb_request *req)
 {
 	size_t smblen = smb_len(req->inbuf);
 	size_t already_used, length_needed;
 	uint8_t chain_cmd;
 	uint32_t chain_offset;	 
 
 	uint8_t wct;
 	uint16_t *vwv;
 	uint16_t buflen;
 	uint8_t *buf;
 
 	if (IVAL(req->outbuf, smb_rcls) != 0) {
 		fixup_chain_error_packet(req);
 	}
 
 	 
  
         if ((req->wct < 2) || (CVAL(req->outbuf, smb_wct) < 2)) {
//                if (req->chain_outbuf == NULL) {
//                        req->chain_outbuf = TALLOC_REALLOC_ARRAY(
//                                req, req->outbuf, uint8_t,
//                                smb_len(req->outbuf) + 4);
//                        if (req->chain_outbuf == NULL) {
//                                smb_panic("talloc failed");
//                        }
//                }
//                req->outbuf = NULL;
                 goto error;
         }
  
 
 	if (req->chain_outbuf == NULL) {
 		 
 		req->chain_outbuf = TALLOC_REALLOC_ARRAY(
 			req, req->outbuf, uint8_t, smb_len(req->outbuf) + 4);
 		if (req->chain_outbuf == NULL) {
 			goto error;
 		}
 		req->outbuf = NULL;
 	} else {
 		 
 
 	chain_cmd = CVAL(req->vwv+0, 0);
 	chain_offset = SVAL(req->vwv+1, 0);
 
 	if (chain_cmd == 0xff) {
 		 
 		smb_setlen((char *)(req->chain_outbuf),
 			   talloc_get_size(req->chain_outbuf) - 4);
 
 		if (!srv_send_smb(smbd_server_fd(), (char *)req->chain_outbuf,
 				  true, req->seqnum+1,
 				  IS_CONN_ENCRYPTED(req->conn)
 				  ||req->encrypted,
 				  &req->pcd)) {
 			exit_server_cleanly("chain_reply: srv_send_smb "
 					    "failed.");
 		}
 		TALLOC_FREE(req->chain_outbuf);
 		req->done = true;
 		return;
 	}
 
 	 
 	SMB_PERFCOUNT_ADD(&req->pcd);
 	SMB_PERFCOUNT_SET_OP(&req->pcd, chain_cmd);
 	SMB_PERFCOUNT_SET_MSGLEN_IN(&req->pcd, smblen);
 
 	 
 
 	already_used = PTR_DIFF(req->buf+req->buflen, smb_base(req->inbuf));
 	if (chain_offset < already_used) {
 		goto error;
 	}
 
 	 
 
 	length_needed = chain_offset+1;	 
 	if (length_needed > smblen) {
 		goto error;
 	}
 
 	 
 
 	wct = CVAL(smb_base(req->inbuf), chain_offset);
 
 	 
 
 	length_needed += (wct+1)*sizeof(uint16_t);  
 	if (length_needed > smblen) {
 		goto error;
 	}
 	vwv = (uint16_t *)(smb_base(req->inbuf) + chain_offset + 1);
 
 	 
 
 	buflen = SVAL(vwv+wct, 0);
 
 	 
 
 	length_needed += buflen;
 	if (length_needed > smblen) {
 		goto error;
 	}
 	buf = (uint8_t *)(vwv+wct+1);
 
 	req->cmd = chain_cmd;
 	req->wct = wct;
 	req->vwv = vwv;
 	req->buflen = buflen;
 	req->buf = buf;
 
 	switch_message(chain_cmd, req, smblen);
 
 	if (req->outbuf == NULL) {
 		 
 		return;
 	}
 
 	 
 	goto done;
 
  error:
 	 
 	reply_force_doserror(req, ERRSRV, ERRerror);
 	fixup_chain_error_packet(req);
 
  done:
 	 
 	SSVAL(req->chain_outbuf, smb_flg2,
 	      (SVAL(req->chain_outbuf, smb_flg2) & ~FLAGS2_32_BIT_ERROR_CODES)
 	      | (SVAL(req->outbuf, smb_flg2) & FLAGS2_32_BIT_ERROR_CODES));
 
 	 
 	SSVAL(req->chain_outbuf, smb_rcls, SVAL(req->outbuf, smb_rcls));
 	SSVAL(req->chain_outbuf, smb_err, SVAL(req->outbuf, smb_err));
 
 	if (!smb_splice_chain(&req->chain_outbuf,
 			      CVAL(req->outbuf, smb_com),
 			      CVAL(req->outbuf, smb_wct),
 			      (uint16_t *)(req->outbuf + smb_vwv),
 			      0, smb_buflen(req->outbuf),
 			      (uint8_t *)smb_buf(req->outbuf))) {
 		exit_server_cleanly("chain_reply: smb_splice_chain failed\n");
 	}
 	TALLOC_FREE(req->outbuf);
 
 	smb_setlen((char *)(req->chain_outbuf),
 		   talloc_get_size(req->chain_outbuf) - 4);
 
 	show_msg((char *)(req->chain_outbuf));
 
 	if (!srv_send_smb(smbd_server_fd(), (char *)req->chain_outbuf,
 			  true, req->seqnum+1,
 			  IS_CONN_ENCRYPTED(req->conn)||req->encrypted,
 			  &req->pcd)) {
 		exit_server_cleanly("construct_reply: srv_send_smb failed.");
 	}
 	TALLOC_FREE(req->chain_outbuf);
 	req->done = true;
 }