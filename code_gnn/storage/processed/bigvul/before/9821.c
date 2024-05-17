int http_wait_for_request_body(struct session *s, struct channel *req, int an_bit)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &s->txn.req;

	 

	if (msg->msg_state < HTTP_MSG_CHUNK_SIZE) {
		 
		if (msg->msg_state < HTTP_MSG_BODY)
			goto missing_data;

		if (msg->msg_state < HTTP_MSG_100_SENT) {
			 
			if (msg->flags & HTTP_MSGF_VER_11) {
				struct hdr_ctx ctx;
				ctx.idx = 0;
				 
				if (http_find_header2("Expect", 6, req->buf->p, &txn->hdr_idx, &ctx) &&
				    unlikely(ctx.vlen == 12 && strncasecmp(ctx.line+ctx.val, "100-continue", 12) == 0)) {
					bo_inject(s->rep, http_100_chunk.str, http_100_chunk.len);
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
		 
		if (req->buf->i - msg->sov < msg->body_len)
			goto missing_data;

		 
		goto http_end;
	}

	 

	if (msg->msg_state == HTTP_MSG_CHUNK_SIZE) {
		 
		int ret = http_parse_chunk_size(msg);

		if (!ret)
			goto missing_data;
		else if (ret < 0) {
			session_inc_http_err_ctr(s);
			goto return_bad_req;
		}
	}

	 
	if (msg->msg_state == HTTP_MSG_TRAILERS)
		goto http_end;

	if (req->buf->i - msg->sov >= msg->body_len)    
		goto http_end;

 missing_data:
	 
	if (buffer_full(req->buf, global.tune.maxrewrite))
		goto http_end;

	if ((req->flags & CF_READ_TIMEOUT) || tick_is_expired(req->analyse_exp, now_ms)) {
		txn->status = 408;
		stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_408));

		if (!(s->flags & SN_ERR_MASK))
			s->flags |= SN_ERR_CLITO;
		if (!(s->flags & SN_FINST_MASK))
			s->flags |= SN_FINST_D;
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
	txn->req.msg_state = HTTP_MSG_ERROR;
	txn->status = 400;
	stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_400));

	if (!(s->flags & SN_ERR_MASK))
		s->flags |= SN_ERR_PRXCOND;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= SN_FINST_R;

 return_err_msg:
	req->analysers = 0;
	s->fe->fe_counters.failed_req++;
	if (s->listener->counters)
		s->listener->counters->failed_req++;
	return 0;
}
