int http_sync_req_state(struct session *s)
{
	struct channel *chn = s->req;
	struct http_txn *txn = &s->txn;
	unsigned int old_flags = chn->flags;
	unsigned int old_state = txn->req.msg_state;

	if (unlikely(txn->req.msg_state < HTTP_MSG_BODY))
		return 0;

	if (txn->req.msg_state == HTTP_MSG_DONE) {
		 
		if (((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_SCL) &&
		    ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) &&
		    !(s->be->options & PR_O_ABRT_CLOSE) &&
		    txn->meth != HTTP_METH_POST)
			channel_dont_read(chn);

		 
		chn->cons->flags |= SI_FL_NOHALF;

		if (txn->rsp.msg_state == HTTP_MSG_ERROR)
			goto wait_other_side;

		if (txn->rsp.msg_state < HTTP_MSG_DONE) {
			 
			goto wait_other_side;
		}

		if (txn->rsp.msg_state == HTTP_MSG_TUNNEL) {
			 
			channel_auto_read(chn);
			txn->req.msg_state = HTTP_MSG_TUNNEL;
			chn->flags |= CF_NEVER_WAIT;
			goto wait_other_side;
		}

		 

		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL) {
			 
			if (!(chn->flags & (CF_SHUTW|CF_SHUTW_NOW)))
				channel_shutw_now(chn);
		}
		else if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_CLO) {
			 
			if (!(chn->flags & (CF_SHUTW|CF_SHUTW_NOW))) {
				channel_shutr_now(chn);
				channel_shutw_now(chn);
			}
		}
		else {
			 
			if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_TUN) {
				channel_auto_read(chn);
				txn->req.msg_state = HTTP_MSG_TUNNEL;
				chn->flags |= CF_NEVER_WAIT;
			}
		}

		if (chn->flags & (CF_SHUTW|CF_SHUTW_NOW)) {
			 
			chn->cons->flags |= SI_FL_NOLINGER;   

			if (!channel_is_empty(chn)) {
				txn->req.msg_state = HTTP_MSG_CLOSING;
				goto http_msg_closing;
			}
			else {
				txn->req.msg_state = HTTP_MSG_CLOSED;
				goto http_msg_closed;
			}
		}
		goto wait_other_side;
	}

	if (txn->req.msg_state == HTTP_MSG_CLOSING) {
	http_msg_closing:
		 
		if (channel_is_empty(chn)) {
			txn->req.msg_state = HTTP_MSG_CLOSED;
			goto http_msg_closed;
		}
		else if (chn->flags & CF_SHUTW) {
			txn->req.msg_state = HTTP_MSG_ERROR;
			goto wait_other_side;
		}
	}

	if (txn->req.msg_state == HTTP_MSG_CLOSED) {
	http_msg_closed:
		 
		if (((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_SCL) &&
		    ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_KAL) &&
		    !(s->be->options & PR_O_ABRT_CLOSE))
			channel_dont_read(chn);
		goto wait_other_side;
	}

 wait_other_side:
	return txn->req.msg_state != old_state || chn->flags != old_flags;
}
