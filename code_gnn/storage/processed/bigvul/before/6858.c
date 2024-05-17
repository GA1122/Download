int http_sync_res_state(struct stream *s)
{
	struct channel *chn = &s->res;
	struct http_txn *txn = s->txn;
	unsigned int old_flags = chn->flags;
	unsigned int old_state = txn->rsp.msg_state;

	if (unlikely(txn->rsp.msg_state < HTTP_MSG_DONE))
		return 0;

	if (txn->rsp.msg_state == HTTP_MSG_DONE) {
		 
		 

		if (txn->req.msg_state == HTTP_MSG_ERROR)
			goto wait_other_side;

		if (txn->req.msg_state < HTTP_MSG_DONE) {
			 
			goto wait_other_side;
		}

		 

		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL) {
			 
			if (!(chn->flags & (CF_SHUTR|CF_SHUTR_NOW)))
				channel_shutr_now(chn);
		}
		else if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_CLO) {
			 
			if (!(chn->flags & (CF_SHUTW|CF_SHUTW_NOW))) {
				channel_shutr_now(chn);
				channel_shutw_now(chn);
			}
		}
		else {
			 
			channel_auto_read(chn);
			chn->flags |= CF_NEVER_WAIT;
			if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_TUN)
				txn->rsp.msg_state = HTTP_MSG_TUNNEL;
		}

		goto check_channel_flags;
	}

	if (txn->rsp.msg_state == HTTP_MSG_CLOSING) {
	http_msg_closing:
		 
		if (channel_is_empty(chn)) {
			txn->rsp.msg_state = HTTP_MSG_CLOSED;
			goto http_msg_closed;
		}
		else if (chn->flags & CF_SHUTW) {
			txn->rsp.err_state = txn->rsp.msg_state;
			txn->rsp.msg_state = HTTP_MSG_ERROR;
			HA_ATOMIC_ADD(&s->be->be_counters.cli_aborts, 1);
			if (objt_server(s->target))
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.cli_aborts, 1);
		}
		goto wait_other_side;
	}

	if (txn->rsp.msg_state == HTTP_MSG_CLOSED) {
	http_msg_closed:
		 
		channel_truncate(chn);
		channel_auto_close(chn);
		channel_auto_read(chn);
		goto wait_other_side;
	}

 check_channel_flags:
	 
	if (chn->flags & (CF_SHUTW|CF_SHUTW_NOW)) {
		 
		txn->rsp.msg_state = HTTP_MSG_CLOSING;
		goto http_msg_closing;
	}

 wait_other_side:
	 
	if (!channel_is_empty(chn))
		chn->flags |= CF_SEND_DONTWAIT;
	return txn->rsp.msg_state != old_state || chn->flags != old_flags;
}
