int http_wait_for_request(struct stream *s, struct channel *req, int an_bit)
{
	 

	int cur_idx;
	struct session *sess = s->sess;
	struct http_txn *txn = s->txn;
	struct http_msg *msg = &txn->req;
	struct hdr_ctx ctx;

	DPRINTF(stderr,"[%u] %s: stream=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		req,
		req->rex, req->wex,
		req->flags,
		req->buf->i,
		req->analysers);

	 
	s->srv_error = http_return_srv_error;

	 
	if (buffer_not_empty(req->buf) && s->logs.t_idle == -1)
		s->logs.t_idle = tv_ms_elapsed(&s->logs.tv_accept, &now) - s->logs.t_handshake;

	 
	if (buffer_not_empty(req->buf) && msg->msg_state < HTTP_MSG_ERROR) {
		if (txn->flags & TX_NOT_FIRST) {
			if (unlikely(!channel_is_rewritable(req))) {
				if (req->flags & (CF_SHUTW|CF_SHUTW_NOW|CF_WRITE_ERROR|CF_WRITE_TIMEOUT))
					goto failed_keep_alive;
				 
				channel_dont_connect(req);
				req->flags |= CF_READ_DONTWAIT;  
				req->flags |= CF_WAKE_WRITE;
				return 0;
			}
			if (unlikely(bi_end(req->buf) < b_ptr(req->buf, msg->next) ||
			             bi_end(req->buf) > req->buf->data + req->buf->size - global.tune.maxrewrite))
				buffer_slow_realign(req->buf);
		}

		if (likely(msg->next < req->buf->i))  
			http_msg_analyzer(msg, &txn->hdr_idx);
	}

	 
	if (unlikely((global.mode & MODE_DEBUG) &&
		     (!(global.mode & MODE_QUIET) || (global.mode & MODE_VERBOSE)) &&
		     msg->msg_state >= HTTP_MSG_BODY)) {
		char *eol, *sol;

		sol = req->buf->p;
		 
		eol = sol + (msg->sl.rq.l ? msg->sl.rq.l : req->buf->i);
		debug_hdr("clireq", s, sol, eol);

		sol += hdr_idx_first_pos(&txn->hdr_idx);
		cur_idx = hdr_idx_first_idx(&txn->hdr_idx);

		while (cur_idx) {
			eol = sol + txn->hdr_idx.v[cur_idx].len;
			debug_hdr("clihdr", s, sol, eol);
			sol = eol + txn->hdr_idx.v[cur_idx].cr + 1;
			cur_idx = txn->hdr_idx.v[cur_idx].next;
		}
	}


	 

	if (unlikely(msg->msg_state < HTTP_MSG_BODY)) {
		 
		if (unlikely(msg->msg_state == HTTP_MSG_ERROR)) {
			stream_inc_http_req_ctr(s);
			stream_inc_http_err_ctr(s);
			proxy_inc_fe_req_ctr(sess->fe);
			goto return_bad_req;
		}

		 
		if (unlikely(buffer_full(req->buf, global.tune.maxrewrite))) {
			 
			stream_inc_http_req_ctr(s);
			stream_inc_http_err_ctr(s);
			proxy_inc_fe_req_ctr(sess->fe);
			if (msg->err_pos < 0)
				msg->err_pos = req->buf->i;
			goto return_bad_req;
		}

		 
		else if (req->flags & CF_READ_ERROR) {
			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_CLICL;

			if (txn->flags & TX_WAIT_NEXT_RQ)
				goto failed_keep_alive;

			if (sess->fe->options & PR_O_IGNORE_PRB)
				goto failed_keep_alive;

			 
			if (msg->err_pos >= 0) {
				http_capture_bad_message(sess->fe, &sess->fe->invalid_req, s, msg, msg->err_state, sess->fe);
				stream_inc_http_err_ctr(s);
			}

			txn->status = 400;
			msg->err_state = msg->msg_state;
			msg->msg_state = HTTP_MSG_ERROR;
			http_reply_and_close(s, txn->status, NULL);
			req->analysers &= AN_REQ_FLT_END;
			stream_inc_http_req_ctr(s);
			proxy_inc_fe_req_ctr(sess->fe);
			HA_ATOMIC_ADD(&sess->fe->fe_counters.failed_req, 1);
			if (sess->listener->counters)
				HA_ATOMIC_ADD(&sess->listener->counters->failed_req, 1);

			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_R;
			return 0;
		}

		 
		else if (req->flags & CF_READ_TIMEOUT || tick_is_expired(req->analyse_exp, now_ms)) {
			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_CLITO;

			if (txn->flags & TX_WAIT_NEXT_RQ)
				goto failed_keep_alive;

			if (sess->fe->options & PR_O_IGNORE_PRB)
				goto failed_keep_alive;

			 
			if (msg->err_pos >= 0) {
				http_capture_bad_message(sess->fe, &sess->fe->invalid_req, s, msg, msg->err_state, sess->fe);
				stream_inc_http_err_ctr(s);
			}
			txn->status = 408;
			msg->err_state = msg->msg_state;
			msg->msg_state = HTTP_MSG_ERROR;
			http_reply_and_close(s, txn->status, http_error_message(s));
			req->analysers &= AN_REQ_FLT_END;

			stream_inc_http_req_ctr(s);
			proxy_inc_fe_req_ctr(sess->fe);
			HA_ATOMIC_ADD(&sess->fe->fe_counters.failed_req, 1);
			if (sess->listener->counters)
				HA_ATOMIC_ADD(&sess->listener->counters->failed_req, 1);

			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_R;
			return 0;
		}

		 
		else if (req->flags & CF_SHUTR) {
			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_CLICL;

			if (txn->flags & TX_WAIT_NEXT_RQ)
				goto failed_keep_alive;

			if (sess->fe->options & PR_O_IGNORE_PRB)
				goto failed_keep_alive;

			if (msg->err_pos >= 0)
				http_capture_bad_message(sess->fe, &sess->fe->invalid_req, s, msg, msg->err_state, sess->fe);
			txn->status = 400;
			msg->err_state = msg->msg_state;
			msg->msg_state = HTTP_MSG_ERROR;
			http_reply_and_close(s, txn->status, http_error_message(s));
			req->analysers &= AN_REQ_FLT_END;
			stream_inc_http_err_ctr(s);
			stream_inc_http_req_ctr(s);
			proxy_inc_fe_req_ctr(sess->fe);
			HA_ATOMIC_ADD(&sess->fe->fe_counters.failed_req, 1);
			if (sess->listener->counters)
				HA_ATOMIC_ADD(&sess->listener->counters->failed_req, 1);

			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_R;
			return 0;
		}

		channel_dont_connect(req);
		req->flags |= CF_READ_DONTWAIT;  
		s->res.flags &= ~CF_EXPECT_MORE;  
#ifdef TCP_QUICKACK
		if (sess->listener->options & LI_O_NOQUICKACK && req->buf->i &&
		    objt_conn(sess->origin) && conn_ctrl_ready(__objt_conn(sess->origin))) {
			 
			setsockopt(__objt_conn(sess->origin)->handle.fd, IPPROTO_TCP, TCP_QUICKACK, &one, sizeof(one));
		}
#endif

		if ((msg->msg_state != HTTP_MSG_RQBEFORE) && (txn->flags & TX_WAIT_NEXT_RQ)) {
			 
			txn->flags &= ~TX_WAIT_NEXT_RQ;
			req->analyse_exp = TICK_ETERNITY;
		}

		 
		if (!tick_isset(req->analyse_exp)) {
			if ((msg->msg_state == HTTP_MSG_RQBEFORE) &&
			    (txn->flags & TX_WAIT_NEXT_RQ) &&
			    tick_isset(s->be->timeout.httpka))
				req->analyse_exp = tick_add(now_ms, s->be->timeout.httpka);
			else
				req->analyse_exp = tick_add_ifset(now_ms, s->be->timeout.httpreq);
		}

		 
		return 0;

	failed_keep_alive:
		 
		txn->status = 0;
		msg->msg_state = HTTP_MSG_RQBEFORE;
		req->analysers &= AN_REQ_FLT_END;
		s->logs.logwait = 0;
		s->logs.level = 0;
		s->res.flags &= ~CF_EXPECT_MORE;  
		http_reply_and_close(s, txn->status, NULL);
		return 0;
	}

	 

	stream_inc_http_req_ctr(s);
	proxy_inc_fe_req_ctr(sess->fe);  

	if (txn->flags & TX_WAIT_NEXT_RQ) {
		 
		txn->flags &= ~TX_WAIT_NEXT_RQ;
		req->analyse_exp = TICK_ETERNITY;
	}


	 
	if (unlikely(msg->err_pos >= 0))
		http_capture_bad_message(sess->fe, &sess->fe->invalid_req, s, msg, msg->err_state, sess->fe);

	 
	txn->meth = find_http_meth(req->buf->p, msg->sl.rq.m_l);

	 
	if (txn->meth == HTTP_METH_GET || txn->meth == HTTP_METH_HEAD)
		s->flags |= SF_REDIRECTABLE;
	else if (txn->meth == HTTP_METH_OTHER &&
		 msg->sl.rq.m_l == 3 && memcmp(req->buf->p, "PRI", 3) == 0) {
		 
		msg->err_pos = 0;
		goto return_bad_req;
	}

	 
	if (unlikely((sess->fe->monitor_uri_len != 0) &&
		     (sess->fe->monitor_uri_len == msg->sl.rq.u_l) &&
		     !memcmp(req->buf->p + msg->sl.rq.u,
			     sess->fe->monitor_uri,
			     sess->fe->monitor_uri_len))) {
		 
		struct acl_cond *cond;

		s->flags |= SF_MONITOR;
		HA_ATOMIC_ADD(&sess->fe->fe_counters.intercepted_req, 1);

		 
		list_for_each_entry(cond, &sess->fe->mon_fail_cond, list) {
			int ret = acl_exec_cond(cond, sess->fe, sess, s, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);

			ret = acl_pass(ret);
			if (cond->pol == ACL_COND_UNLESS)
				ret = !ret;

			if (ret) {
				 
				txn->status = 503;
				http_reply_and_close(s, txn->status, http_error_message(s));
				if (!(s->flags & SF_ERR_MASK))
					s->flags |= SF_ERR_LOCAL;  
				goto return_prx_cond;
			}
		}

		 
		txn->status = 200;
		http_reply_and_close(s, txn->status, http_error_message(s));
		if (!(s->flags & SF_ERR_MASK))
			s->flags |= SF_ERR_LOCAL;  
		goto return_prx_cond;
	}

	 
	if (unlikely(s->logs.logwait & LW_REQ)) {
		 
		if ((txn->uri = pool_alloc(pool_head_requri)) != NULL) {
			int urilen = msg->sl.rq.l;

			if (urilen >= global.tune.requri_len )
				urilen = global.tune.requri_len - 1;
			memcpy(txn->uri, req->buf->p, urilen);
			txn->uri[urilen] = 0;

			if (!(s->logs.logwait &= ~(LW_REQ|LW_INIT)))
				s->do_log(s);
		} else {
			ha_alert("HTTP logging : out of memory.\n");
		}
	}

	 
	if (!(sess->fe->options2 & PR_O2_REQBUG_OK)) {
		if (msg->sl.rq.v_l != 8) {
			msg->err_pos = msg->sl.rq.v;
			goto return_bad_req;
		}

		if (req->buf->p[msg->sl.rq.v + 4] != '/' ||
		    !isdigit((unsigned char)req->buf->p[msg->sl.rq.v + 5]) ||
		    req->buf->p[msg->sl.rq.v + 6] != '.' ||
		    !isdigit((unsigned char)req->buf->p[msg->sl.rq.v + 7])) {
			msg->err_pos = msg->sl.rq.v + 4;
			goto return_bad_req;
		}
	}
	else {
		 
		if (unlikely(msg->sl.rq.v_l == 0) && !http_upgrade_v09_to_v10(txn))
			goto return_bad_req;
	}

	 
	if ((msg->sl.rq.v_l == 8) &&
	    ((req->buf->p[msg->sl.rq.v + 5] > '1') ||
	     ((req->buf->p[msg->sl.rq.v + 5] == '1') &&
	      (req->buf->p[msg->sl.rq.v + 7] >= '1'))))
		msg->flags |= HTTP_MSGF_VER_11;

	 
	txn->flags &= ~(TX_HDR_CONN_PRS | TX_HDR_CONN_CLO | TX_HDR_CONN_KAL | TX_HDR_CONN_UPG);

	 
	if ((sess->fe->options2 & PR_O2_USE_PXHDR) &&
	    req->buf->p[msg->sl.rq.u] != '/' && req->buf->p[msg->sl.rq.u] != '*')
		txn->flags |= TX_USE_PX_CONN;

	 
	msg->flags &= ~HTTP_MSGF_XFER_LEN;

	 
	if (unlikely((s->logs.logwait & LW_REQHDR) && s->req_cap))
		capture_headers(req->buf->p, &txn->hdr_idx,
				s->req_cap, sess->fe->req_cap);

	 

	ctx.idx = 0;
	 
	while (http_find_header2("Transfer-Encoding", 17, req->buf->p, &txn->hdr_idx, &ctx)) {
		if (ctx.vlen == 7 && strncasecmp(ctx.line + ctx.val, "chunked", 7) == 0)
			msg->flags |= HTTP_MSGF_TE_CHNK;
		else if (msg->flags & HTTP_MSGF_TE_CHNK) {
			 
			goto return_bad_req;
		}
	}

	 
	ctx.idx = 0;
	if (msg->flags & HTTP_MSGF_TE_CHNK) {
		while (http_find_header2("Content-Length", 14, req->buf->p, &txn->hdr_idx, &ctx))
			http_remove_header2(msg, &txn->hdr_idx, &ctx);
	}
	else while (http_find_header2("Content-Length", 14, req->buf->p, &txn->hdr_idx, &ctx)) {
		signed long long cl;

		if (!ctx.vlen) {
			msg->err_pos = ctx.line + ctx.val - req->buf->p;
			goto return_bad_req;
		}

		if (strl2llrc(ctx.line + ctx.val, ctx.vlen, &cl)) {
			msg->err_pos = ctx.line + ctx.val - req->buf->p;
			goto return_bad_req;  
		}

		if (cl < 0) {
			msg->err_pos = ctx.line + ctx.val - req->buf->p;
			goto return_bad_req;
		}

		if ((msg->flags & HTTP_MSGF_CNT_LEN) && (msg->chunk_len != cl)) {
			msg->err_pos = ctx.line + ctx.val - req->buf->p;
			goto return_bad_req;  
		}

		msg->flags |= HTTP_MSGF_CNT_LEN;
		msg->body_len = msg->chunk_len = cl;
	}

	 
	msg->flags |= HTTP_MSGF_XFER_LEN;

	 
	if (!(txn->flags & TX_HDR_CONN_PRS) ||
	    ((sess->fe->options & PR_O_HTTP_MODE) != (s->be->options & PR_O_HTTP_MODE)))
		http_adjust_conn_mode(s, txn, msg);

	 
	if ((s->be->options & PR_O_WREQ_BODY) &&
	    (msg->body_len || (msg->flags & HTTP_MSGF_TE_CHNK)))
		req->analysers |= AN_REQ_HTTP_BODY;

	 
	if (likely(txn->meth == HTTP_METH_GET ||
		   txn->meth == HTTP_METH_HEAD ||
		   txn->meth == HTTP_METH_OPTIONS ||
		   txn->meth == HTTP_METH_TRACE))
		txn->flags |= TX_CACHEABLE | TX_CACHE_COOK;

	 
	req->analysers &= ~an_bit;
	req->analyse_exp = TICK_ETERNITY;
	return 1;

 return_bad_req:
	 
	if (unlikely(msg->msg_state == HTTP_MSG_ERROR) || msg->err_pos >= 0) {
		 
		http_capture_bad_message(sess->fe, &sess->fe->invalid_req, s, msg, msg->err_state, sess->fe);
	}

	txn->req.err_state = txn->req.msg_state;
	txn->req.msg_state = HTTP_MSG_ERROR;
	txn->status = 400;
	http_reply_and_close(s, txn->status, http_error_message(s));

	HA_ATOMIC_ADD(&sess->fe->fe_counters.failed_req, 1);
	if (sess->listener->counters)
		HA_ATOMIC_ADD(&sess->listener->counters->failed_req, 1);

 return_prx_cond:
	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_PRXCOND;
	if (!(s->flags & SF_FINST_MASK))
		s->flags |= SF_FINST_R;

	req->analysers &= AN_REQ_FLT_END;
	req->analyse_exp = TICK_ETERNITY;
	return 0;
}
