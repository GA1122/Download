int http_wait_for_request(struct session *s, struct channel *req, int an_bit)
{
	 

	int cur_idx;
	int use_close_only;
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &txn->req;
	struct hdr_ctx ctx;

	DPRINTF(stderr,"[%u] %s: session=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		req,
		req->rex, req->wex,
		req->flags,
		req->buf->i,
		req->analysers);

	 
	s->srv_error = http_return_srv_error;

	 
	if (buffer_not_empty(req->buf) && msg->msg_state < HTTP_MSG_ERROR) {
		if (txn->flags & TX_NOT_FIRST) {
			if (unlikely(!channel_reserved(req))) {
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

		 
		if ((txn->flags & TX_NOT_FIRST) &&
		    unlikely(!channel_reserved(s->rep) ||
			     bi_end(s->rep->buf) < b_ptr(s->rep->buf, txn->rsp.next) ||
			     bi_end(s->rep->buf) > s->rep->buf->data + s->rep->buf->size - global.tune.maxrewrite)) {
			if (s->rep->buf->o) {
				if (s->rep->flags & (CF_SHUTW|CF_SHUTW_NOW|CF_WRITE_ERROR|CF_WRITE_TIMEOUT))
					goto failed_keep_alive;
				 
				channel_dont_connect(req);
				s->rep->flags &= ~CF_EXPECT_MORE;  
				s->rep->flags |= CF_WAKE_WRITE;
				s->rep->analysers |= an_bit;  
				return 0;
			}
		}

		if (likely(msg->next < req->buf->i))  
			http_msg_analyzer(msg, &txn->hdr_idx);
	}

	 
	if (unlikely((global.mode & MODE_DEBUG) &&
		     (!(global.mode & MODE_QUIET) || (global.mode & MODE_VERBOSE)) &&
		     (msg->msg_state >= HTTP_MSG_BODY || msg->msg_state == HTTP_MSG_ERROR))) {
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
			session_inc_http_req_ctr(s);
			session_inc_http_err_ctr(s);
			proxy_inc_fe_req_ctr(s->fe);
			goto return_bad_req;
		}

		 
		if (unlikely(buffer_full(req->buf, global.tune.maxrewrite))) {
			 
			session_inc_http_req_ctr(s);
			session_inc_http_err_ctr(s);
			proxy_inc_fe_req_ctr(s->fe);
			if (msg->err_pos < 0)
				msg->err_pos = req->buf->i;
			goto return_bad_req;
		}

		 
		else if (req->flags & CF_READ_ERROR) {
			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_CLICL;

			if (txn->flags & TX_WAIT_NEXT_RQ)
				goto failed_keep_alive;

			 
			if (msg->err_pos >= 0) {
				http_capture_bad_message(&s->fe->invalid_req, s, msg, msg->msg_state, s->fe);
				session_inc_http_err_ctr(s);
			}

			txn->status = 400;
			stream_int_retnclose(req->prod, NULL);
			msg->msg_state = HTTP_MSG_ERROR;
			req->analysers = 0;

			session_inc_http_req_ctr(s);
			proxy_inc_fe_req_ctr(s->fe);
			s->fe->fe_counters.failed_req++;
			if (s->listener->counters)
				s->listener->counters->failed_req++;

			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_R;
			return 0;
		}

		 
		else if (req->flags & CF_READ_TIMEOUT || tick_is_expired(req->analyse_exp, now_ms)) {
			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_CLITO;

			if (txn->flags & TX_WAIT_NEXT_RQ)
				goto failed_keep_alive;

			 
			if (msg->err_pos >= 0) {
				http_capture_bad_message(&s->fe->invalid_req, s, msg, msg->msg_state, s->fe);
				session_inc_http_err_ctr(s);
			}
			txn->status = 408;
			stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_408));
			msg->msg_state = HTTP_MSG_ERROR;
			req->analysers = 0;

			session_inc_http_req_ctr(s);
			proxy_inc_fe_req_ctr(s->fe);
			s->fe->fe_counters.failed_req++;
			if (s->listener->counters)
				s->listener->counters->failed_req++;

			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_R;
			return 0;
		}

		 
		else if (req->flags & CF_SHUTR) {
			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_CLICL;

			if (txn->flags & TX_WAIT_NEXT_RQ)
				goto failed_keep_alive;

			if (msg->err_pos >= 0)
				http_capture_bad_message(&s->fe->invalid_req, s, msg, msg->msg_state, s->fe);
			txn->status = 400;
			stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_400));
			msg->msg_state = HTTP_MSG_ERROR;
			req->analysers = 0;

			session_inc_http_err_ctr(s);
			session_inc_http_req_ctr(s);
			proxy_inc_fe_req_ctr(s->fe);
			s->fe->fe_counters.failed_req++;
			if (s->listener->counters)
				s->listener->counters->failed_req++;

			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_R;
			return 0;
		}

		channel_dont_connect(req);
		req->flags |= CF_READ_DONTWAIT;  
		s->rep->flags &= ~CF_EXPECT_MORE;  
#ifdef TCP_QUICKACK
		if (s->listener->options & LI_O_NOQUICKACK && req->buf->i && objt_conn(s->req->prod->end) && conn_ctrl_ready(__objt_conn(s->req->prod->end))) {
			 
			setsockopt(__objt_conn(s->req->prod->end)->t.sock.fd, IPPROTO_TCP, TCP_QUICKACK, &one, sizeof(one));
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
		req->analysers = 0;
		s->logs.logwait = 0;
		s->logs.level = 0;
		s->rep->flags &= ~CF_EXPECT_MORE;  
		stream_int_retnclose(req->prod, NULL);
		return 0;
	}

	 

	session_inc_http_req_ctr(s);
	proxy_inc_fe_req_ctr(s->fe);  

	if (txn->flags & TX_WAIT_NEXT_RQ) {
		 
		txn->flags &= ~TX_WAIT_NEXT_RQ;
		req->analyse_exp = TICK_ETERNITY;
	}


	 
	if (unlikely(msg->err_pos >= 0))
		http_capture_bad_message(&s->fe->invalid_req, s, msg, msg->msg_state, s->fe);

	 
	txn->meth = find_http_meth(req->buf->p, msg->sl.rq.m_l);

	 
	if (txn->meth == HTTP_METH_GET || txn->meth == HTTP_METH_HEAD)
		s->flags |= SN_REDIRECTABLE;

	 
	if (unlikely((s->fe->monitor_uri_len != 0) &&
		     (s->fe->monitor_uri_len == msg->sl.rq.u_l) &&
		     !memcmp(req->buf->p + msg->sl.rq.u,
			     s->fe->monitor_uri,
			     s->fe->monitor_uri_len))) {
		 
		struct acl_cond *cond;

		s->flags |= SN_MONITOR;
		s->fe->fe_counters.intercepted_req++;

		 
		list_for_each_entry(cond, &s->fe->mon_fail_cond, list) {
			int ret = acl_exec_cond(cond, s->fe, s, txn, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);

			ret = acl_pass(ret);
			if (cond->pol == ACL_COND_UNLESS)
				ret = !ret;

			if (ret) {
				 
				txn->status = 503;
				stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_503));
				if (!(s->flags & SN_ERR_MASK))
					s->flags |= SN_ERR_LOCAL;  
				goto return_prx_cond;
			}
		}

		 
		txn->status = 200;
		stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_200));
		if (!(s->flags & SN_ERR_MASK))
			s->flags |= SN_ERR_LOCAL;  
		goto return_prx_cond;
	}

	 
	if (unlikely(s->logs.logwait & LW_REQ)) {
		 
		if ((txn->uri = pool_alloc2(pool2_requri)) != NULL) {
			int urilen = msg->sl.rq.l;

			if (urilen >= REQURI_LEN)
				urilen = REQURI_LEN - 1;
			memcpy(txn->uri, req->buf->p, urilen);
			txn->uri[urilen] = 0;

			if (!(s->logs.logwait &= ~(LW_REQ|LW_INIT)))
				s->do_log(s);
		} else {
			Alert("HTTP logging : out of memory.\n");
		}
	}

	 
	if (unlikely(msg->sl.rq.v_l == 0) && !http_upgrade_v09_to_v10(txn))
		goto return_bad_req;

	 
	if ((msg->sl.rq.v_l == 8) &&
	    ((req->buf->p[msg->sl.rq.v + 5] > '1') ||
	     ((req->buf->p[msg->sl.rq.v + 5] == '1') &&
	      (req->buf->p[msg->sl.rq.v + 7] >= '1'))))
		msg->flags |= HTTP_MSGF_VER_11;

	 
	txn->flags &= ~(TX_HDR_CONN_PRS | TX_HDR_CONN_CLO | TX_HDR_CONN_KAL | TX_HDR_CONN_UPG);

	 
	if ((s->fe->options2 & PR_O2_USE_PXHDR) &&
	    req->buf->p[msg->sl.rq.u] != '/' && req->buf->p[msg->sl.rq.u] != '*')
		txn->flags |= TX_USE_PX_CONN;

	 
	msg->flags &= ~HTTP_MSGF_XFER_LEN;

	 
	if (unlikely((s->logs.logwait & LW_REQHDR) && txn->req.cap))
		capture_headers(req->buf->p, &txn->hdr_idx,
				txn->req.cap, s->fe->req_cap);

	 

	use_close_only = 0;
	ctx.idx = 0;
	 
	while ((msg->flags & HTTP_MSGF_VER_11) &&
	       http_find_header2("Transfer-Encoding", 17, req->buf->p, &txn->hdr_idx, &ctx)) {
		if (ctx.vlen == 7 && strncasecmp(ctx.line + ctx.val, "chunked", 7) == 0)
			msg->flags |= (HTTP_MSGF_TE_CHNK | HTTP_MSGF_XFER_LEN);
		else if (msg->flags & HTTP_MSGF_TE_CHNK) {
			 
			use_close_only = 1;
			msg->flags &= ~(HTTP_MSGF_TE_CHNK | HTTP_MSGF_XFER_LEN);
			break;
		}
	}

	ctx.idx = 0;
	while (!(msg->flags & HTTP_MSGF_TE_CHNK) && !use_close_only &&
	       http_find_header2("Content-Length", 14, req->buf->p, &txn->hdr_idx, &ctx)) {
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

		msg->flags |= HTTP_MSGF_CNT_LEN | HTTP_MSGF_XFER_LEN;
		msg->body_len = msg->chunk_len = cl;
	}

	 
	if (!use_close_only)
		msg->flags |= HTTP_MSGF_XFER_LEN;

	 
	if (!(txn->flags & TX_HDR_CONN_PRS) ||
	    ((s->fe->options & PR_O_HTTP_MODE) != (s->be->options & PR_O_HTTP_MODE))) {
		int tmp = TX_CON_WANT_KAL;

		if (!((s->fe->options2|s->be->options2) & PR_O2_FAKE_KA)) {
			if ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_TUN ||
			    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_TUN)
				tmp = TX_CON_WANT_TUN;

			if ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
			    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL)
				tmp = TX_CON_WANT_TUN;
		}

		if ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_SCL ||
		    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_SCL) {
			 
			if ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
			    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL)
				tmp = TX_CON_WANT_CLO;
			else
				tmp = TX_CON_WANT_SCL;
		}

		if ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_FCL ||
		    (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_FCL)
			tmp = TX_CON_WANT_CLO;

		if ((txn->flags & TX_CON_WANT_MSK) < tmp)
			txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | tmp;

		if (!(txn->flags & TX_HDR_CONN_PRS) &&
		    (txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN) {
			 
			int to_del = 0;
			if ((msg->flags & HTTP_MSGF_VER_11) ||
			    ((txn->flags & TX_CON_WANT_MSK) >= TX_CON_WANT_SCL &&
			     !((s->fe->options2|s->be->options2) & PR_O2_FAKE_KA)))
				to_del |= 2;  
			if (!(msg->flags & HTTP_MSGF_VER_11))
				to_del |= 1;  
			http_parse_connection_header(txn, msg, to_del);
		}

		 
		if (((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL ||
		     (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL) &&
		    ((txn->flags & TX_HDR_CONN_CLO) ||                          
		     (!(msg->flags & HTTP_MSGF_VER_11) && !(txn->flags & TX_HDR_CONN_KAL)) ||  
		     !(msg->flags & HTTP_MSGF_XFER_LEN) ||                      
		     s->fe->state == PR_STSTOPPED))                             
		    txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | TX_CON_WANT_CLO;
	}

	 
	req->analysers &= ~an_bit;
	req->analyse_exp = TICK_ETERNITY;
	return 1;

 return_bad_req:
	 
	if (unlikely(msg->msg_state == HTTP_MSG_ERROR) || msg->err_pos >= 0) {
		 
		http_capture_bad_message(&s->fe->invalid_req, s, msg, msg->msg_state, s->fe);
	}

	txn->req.msg_state = HTTP_MSG_ERROR;
	txn->status = 400;
	stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_400));

	s->fe->fe_counters.failed_req++;
	if (s->listener->counters)
		s->listener->counters->failed_req++;

 return_prx_cond:
	if (!(s->flags & SN_ERR_MASK))
		s->flags |= SN_ERR_PRXCOND;
	if (!(s->flags & SN_FINST_MASK))
		s->flags |= SN_FINST_R;

	req->analysers = 0;
	req->analyse_exp = TICK_ETERNITY;
	return 0;
}
