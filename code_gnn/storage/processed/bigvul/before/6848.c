int http_request_forward_body(struct stream *s, struct channel *req, int an_bit)
{
	struct session *sess = s->sess;
	struct http_txn *txn = s->txn;
	struct http_msg *msg = &s->txn->req;
	int ret;

	DPRINTF(stderr,"[%u] %s: stream=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		req,
		req->rex, req->wex,
		req->flags,
		req->buf->i,
		req->analysers);

	if (unlikely(msg->msg_state < HTTP_MSG_BODY))
		return 0;

	if ((req->flags & (CF_READ_ERROR|CF_READ_TIMEOUT|CF_WRITE_ERROR|CF_WRITE_TIMEOUT)) ||
	    ((req->flags & CF_SHUTW) && (req->to_forward || req->buf->o))) {
		 
		msg->err_state = msg->msg_state;
		msg->msg_state = HTTP_MSG_ERROR;
		http_resync_states(s);
		return 1;
	}

	 
	if (msg->msg_state == HTTP_MSG_BODY) {
		msg->msg_state = ((msg->flags & HTTP_MSGF_TE_CHNK)
				  ? HTTP_MSG_CHUNK_SIZE
				  : HTTP_MSG_DATA);

		 
	}

	 
	if (unlikely(msg->flags & HTTP_MSGF_WAIT_CONN)) {
		if (!(s->res.flags & CF_READ_ATTACHED)) {
			channel_auto_connect(req);
			req->flags |= CF_WAKE_CONNECT;
			channel_dont_close(req);  
			goto waiting;
		}
		msg->flags &= ~HTTP_MSGF_WAIT_CONN;
	}

	 
	channel_auto_close(req);

	if (req->to_forward) {
		 
		req->flags |= CF_WAKE_WRITE;
		goto missing_data_or_waiting;
	}

	if (msg->msg_state < HTTP_MSG_DONE) {
		ret = ((msg->flags & HTTP_MSGF_TE_CHNK)
		       ? http_msg_forward_chunked_body(s, msg)
		       : http_msg_forward_body(s, msg));
		if (!ret)
			goto missing_data_or_waiting;
		if (ret < 0)
			goto return_bad_req;
	}

	 
	 
	if ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN)
		channel_dont_close(req);

	http_resync_states(s);
	if (!(req->analysers & an_bit)) {
		if (unlikely(msg->msg_state == HTTP_MSG_ERROR)) {
			if (req->flags & CF_SHUTW) {
				 
				goto aborted_xfer;
			}
			if (msg->err_pos >= 0)
				http_capture_bad_message(sess->fe, &sess->fe->invalid_req, s, msg, msg->err_state, s->be);
			goto return_bad_req;
		}
		return 1;
	}

	 
	if ((s->be->options & PR_O_ABRT_CLOSE) && !(s->si[0].flags & SI_FL_CLEAN_ABRT)) {
		channel_auto_read(req);
		if ((req->flags & (CF_SHUTR|CF_READ_NULL)) &&
		    ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN))
			s->si[1].flags |= SI_FL_NOLINGER;
		channel_auto_close(req);
	}
	else if (s->txn->meth == HTTP_METH_POST) {
		 
		channel_auto_read(req);
	}
	return 0;

 missing_data_or_waiting:
	 
	if (msg->msg_state < HTTP_MSG_ENDING && req->flags & CF_SHUTR) {
		if (!(s->flags & SF_ERR_MASK))
			s->flags |= SF_ERR_CLICL;
		if (!(s->flags & SF_FINST_MASK)) {
			if (txn->rsp.msg_state < HTTP_MSG_ERROR)
				s->flags |= SF_FINST_H;
			else
				s->flags |= SF_FINST_D;
		}

		HA_ATOMIC_ADD(&sess->fe->fe_counters.cli_aborts, 1);
		HA_ATOMIC_ADD(&s->be->be_counters.cli_aborts, 1);
		if (objt_server(s->target))
			HA_ATOMIC_ADD(&objt_server(s->target)->counters.cli_aborts, 1);

		goto return_bad_req_stats_ok;
	}

 waiting:
	 
	if (req->flags & CF_SHUTW)
		goto aborted_xfer;

	 
	if (msg->flags & (HTTP_MSGF_TE_CHNK|HTTP_MSGF_CNT_LEN))
		channel_dont_close(req);

	 
	if (msg->flags & HTTP_MSGF_TE_CHNK)
		req->flags |= CF_EXPECT_MORE;

	return 0;

 return_bad_req:  
	HA_ATOMIC_ADD(&sess->fe->fe_counters.failed_req, 1);
	if (sess->listener->counters)
		HA_ATOMIC_ADD(&sess->listener->counters->failed_req, 1);

 return_bad_req_stats_ok:
	txn->req.err_state = txn->req.msg_state;
	txn->req.msg_state = HTTP_MSG_ERROR;
	if (txn->status) {
		 
		http_reply_and_close(s, txn->status, NULL);
	} else {
		txn->status = 400;
		http_reply_and_close(s, txn->status, http_error_message(s));
	}
	req->analysers   &= AN_REQ_FLT_END;
	s->res.analysers &= AN_RES_FLT_END;  

	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_PRXCOND;
	if (!(s->flags & SF_FINST_MASK)) {
		if (txn->rsp.msg_state < HTTP_MSG_ERROR)
			s->flags |= SF_FINST_H;
		else
			s->flags |= SF_FINST_D;
	}
	return 0;

 aborted_xfer:
	txn->req.err_state = txn->req.msg_state;
	txn->req.msg_state = HTTP_MSG_ERROR;
	if (txn->status) {
		 
		http_reply_and_close(s, txn->status, NULL);
	} else {
		txn->status = 502;
		http_reply_and_close(s, txn->status, http_error_message(s));
	}
	req->analysers   &= AN_REQ_FLT_END;
	s->res.analysers &= AN_RES_FLT_END;  

	HA_ATOMIC_ADD(&sess->fe->fe_counters.srv_aborts, 1);
	HA_ATOMIC_ADD(&s->be->be_counters.srv_aborts, 1);
	if (objt_server(s->target))
		HA_ATOMIC_ADD(&objt_server(s->target)->counters.srv_aborts, 1);

	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_SRVCL;
	if (!(s->flags & SF_FINST_MASK)) {
		if (txn->rsp.msg_state < HTTP_MSG_ERROR)
			s->flags |= SF_FINST_H;
		else
			s->flags |= SF_FINST_D;
	}
	return 0;
}
