int http_sync_req_state(struct stream *s)
{
	struct channel *chn = &s->req;
	struct http_txn *txn = s->txn;
	unsigned int old_flags = chn->flags;
	unsigned int old_state = txn->req.msg_state;

	if (unlikely(txn->req.msg_state < HTTP_MSG_DONE))
		return 0;

	if (txn->req.msg_state == HTTP_MSG_DONE) {
		 
		if (((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_SCL) &&
		    ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) &&
		    (!(s->be->options & PR_O_ABRT_CLOSE) ||
		     (s->si[0].flags & SI_FL_CLEAN_ABRT)) &&
		    txn->meth != HTTP_METH_POST)
			channel_dont_read(chn);

		 
		s->si[1].flags |= SI_FL_NOHALF;

		 
		chn->flags |= CF_NEVER_WAIT;

		if (txn->rsp.msg_state == HTTP_MSG_ERROR)
			goto wait_other_side;

		if (txn->rsp.msg_state < HTTP_MSG_DONE) {
			 
			goto wait_other_side;
		}

		 

		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL) {
			 
			if (!(chn->flags & (CF_SHUTW|CF_SHUTW_NOW)))
				channel_shutw_now(chn);
		}
		else if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_CLO) {
			 
			if (!(txn->rsp.flags & HTTP_MSGF_XFER_LEN) &&
			    txn->rsp.msg_state != HTTP_MSG_CLOSED)
				goto check_channel_flags;

			if (!(chn->flags & (CF_SHUTW|CF_SHUTW_NOW))) {
				channel_shutr_now(chn);
				channel_shutw_now(chn);
			}
		}
		else {
			 
			if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_TUN) {
				channel_auto_read(chn);
				txn->req.msg_state = HTTP_MSG_TUNNEL;
			}
		}

		goto check_channel_flags;
	}

	if (txn->req.msg_state == HTTP_MSG_CLOSING) {
	http_msg_closing:
		 
		if (channel_is_empty(chn)) {
			txn->req.msg_state = HTTP_MSG_CLOSED;
			goto http_msg_closed;
		}
		else if (chn->flags & CF_SHUTW) {
			txn->req.err_state = txn->req.msg_state;
			txn->req.msg_state = HTTP_MSG_ERROR;
		}
		goto wait_other_side;
	}

	if (txn->req.msg_state == HTTP_MSG_CLOSED) {
	http_msg_closed:
		 
		if (txn->rsp.flags & HTTP_MSGF_XFER_LEN)
			s->si[1].flags |= SI_FL_NOLINGER;   

		 
		if (((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_SCL) &&
		    ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) &&
		    (!(s->be->options & PR_O_ABRT_CLOSE) ||
		     (s->si[0].flags & SI_FL_CLEAN_ABRT)))
			channel_dont_read(chn);
		goto wait_other_side;
	}

 check_channel_flags:
	 
	if (chn->flags & (CF_SHUTW|CF_SHUTW_NOW)) {
		 
		txn->req.msg_state = HTTP_MSG_CLOSING;
		goto http_msg_closing;
	}


 wait_other_side:
	return txn->req.msg_state != old_state || chn->flags != old_flags;
}
