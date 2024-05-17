int http_wait_for_request_body(struct stream *s, struct channel *req, int an_bit)
{
	struct session *sess = s->sess;
	struct http_txn *txn = s->txn;
	struct http_msg *msg = &s->txn->req;

	 

	if (msg->msg_state < HTTP_MSG_CHUNK_SIZE) {
		 
		if (msg->msg_state < HTTP_MSG_BODY)
			goto missing_data;

		if (msg->msg_state < HTTP_MSG_100_SENT) {
			 
			if (msg->flags & HTTP_MSGF_VER_11) {
				struct hdr_ctx ctx;
				ctx.idx = 0;
				 
				if (http_find_header2("Expect", 6, req->buf->p, &txn->hdr_idx, &ctx) &&
				    unlikely(ctx.vlen == 12 && strncasecmp(ctx.line+ctx.val, "100-continue", 12) == 0)) {
					co_inject(&s->res, http_100_chunk.str, http_100_chunk.len);
					http_remove_header2(&txn->req, &txn->hdr_idx, &ctx);
				}
			}
			msg->msg_state = HTTP_MSG_100_SENT;
		}

		 
		msg->next = msg->sov;

		if (msg->flags & HTTP_MSGF_TE_CHNK)
			msg->msg_state = HTTP_MSG_CHUNK_SIZE;
		else
			msg->msg_state = HTTP_MSG_DATA;
	}

	if (!(msg->flags & HTTP_MSGF_TE_CHNK)) {
		 
		if (http_body_bytes(msg) < msg->body_len)
			goto missing_data;

		 
		goto http_end;
	}

	 

	if (msg->msg_state == HTTP_MSG_CHUNK_SIZE) {
		 
		unsigned int chunk;
		int ret = h1_parse_chunk_size(req->buf, msg->next, req->buf->i, &chunk);

		if (!ret)
			goto missing_data;
		else if (ret < 0) {
			msg->err_pos = req->buf->i + ret;
			if (msg->err_pos < 0)
				msg->err_pos += req->buf->size;
			stream_inc_http_err_ctr(s);
			goto return_bad_req;
		}

		msg->chunk_len = chunk;
		msg->body_len += chunk;

		msg->sol = ret;
		msg->next += ret;
		msg->msg_state = msg->chunk_len ? HTTP_MSG_DATA : HTTP_MSG_TRAILERS;
	}

	 
	if (msg->msg_state == HTTP_MSG_TRAILERS)
		goto http_end;

	if (http_body_bytes(msg) >= msg->body_len)    
		goto http_end;

 missing_data:
	 
	if (buffer_full(req->buf, global.tune.maxrewrite))
		goto http_end;

	if ((req->flags & CF_READ_TIMEOUT) || tick_is_expired(req->analyse_exp, now_ms)) {
		txn->status = 408;
		http_reply_and_close(s, txn->status, http_error_message(s));

		if (!(s->flags & SF_ERR_MASK))
			s->flags |= SF_ERR_CLITO;
		if (!(s->flags & SF_FINST_MASK))
			s->flags |= SF_FINST_D;
		goto return_err_msg;
	}

	 
	if (!(req->flags & (CF_SHUTR | CF_READ_ERROR))) {
		 
		channel_dont_connect(req);
		if (!tick_isset(req->analyse_exp))
			req->analyse_exp = tick_add_ifset(now_ms, s->be->timeout.httpreq);
		return 0;
	}

 http_end:
	 
	s->logs.tv_request = now;   
	req->analysers &= ~an_bit;
	req->analyse_exp = TICK_ETERNITY;
	return 1;

 return_bad_req:  
	txn->req.err_state = txn->req.msg_state;
	txn->req.msg_state = HTTP_MSG_ERROR;
	txn->status = 400;
	http_reply_and_close(s, txn->status, http_error_message(s));

	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_PRXCOND;
	if (!(s->flags & SF_FINST_MASK))
		s->flags |= SF_FINST_R;

 return_err_msg:
	req->analysers &= AN_REQ_FLT_END;
	HA_ATOMIC_ADD(&sess->fe->fe_counters.failed_req, 1);
	if (sess->listener->counters)
		HA_ATOMIC_ADD(&sess->listener->counters->failed_req, 1);
	return 0;
}
