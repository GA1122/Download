int http_wait_for_response(struct session *s, struct channel *rep, int an_bit)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &txn->rsp;
	struct hdr_ctx ctx;
	int use_close_only;
	int cur_idx;
	int n;

	DPRINTF(stderr,"[%u] %s: session=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		rep,
		rep->rex, rep->wex,
		rep->flags,
		rep->buf->i,
		rep->analysers);

	 

 next_one:
	 
	if (buffer_not_empty(rep->buf) && msg->msg_state < HTTP_MSG_ERROR) {
		if (unlikely(!channel_reserved(rep))) {
			 
			if (rep->flags & (CF_SHUTW|CF_SHUTW_NOW|CF_WRITE_ERROR|CF_WRITE_TIMEOUT))
				goto abort_response;
			channel_dont_close(rep);
			rep->flags |= CF_READ_DONTWAIT;  
			rep->flags |= CF_WAKE_WRITE;
			return 0;
		}

		if (unlikely(bi_end(rep->buf) < b_ptr(rep->buf, msg->next) ||
		             bi_end(rep->buf) > rep->buf->data + rep->buf->size - global.tune.maxrewrite))
			buffer_slow_realign(rep->buf);

		if (likely(msg->next < rep->buf->i))
			http_msg_analyzer(msg, &txn->hdr_idx);
	}

	 
	if (unlikely((global.mode & MODE_DEBUG) &&
		     (!(global.mode & MODE_QUIET) || (global.mode & MODE_VERBOSE)) &&
		     (msg->msg_state >= HTTP_MSG_BODY || msg->msg_state == HTTP_MSG_ERROR))) {
		char *eol, *sol;

		sol = rep->buf->p;
		eol = sol + (msg->sl.st.l ? msg->sl.st.l : rep->buf->i);
		debug_hdr("srvrep", s, sol, eol);

		sol += hdr_idx_first_pos(&txn->hdr_idx);
		cur_idx = hdr_idx_first_idx(&txn->hdr_idx);

		while (cur_idx) {
			eol = sol + txn->hdr_idx.v[cur_idx].len;
			debug_hdr("srvhdr", s, sol, eol);
			sol = eol + txn->hdr_idx.v[cur_idx].cr + 1;
			cur_idx = txn->hdr_idx.v[cur_idx].next;
		}
	}

	 

	if (unlikely(msg->msg_state < HTTP_MSG_BODY)) {
		 
		if (unlikely(msg->msg_state == HTTP_MSG_ERROR)) {
			 
		hdr_response_bad:
			if (msg->msg_state == HTTP_MSG_ERROR || msg->err_pos >= 0)
				http_capture_bad_message(&s->be->invalid_rep, s, msg, msg->msg_state, s->fe);

			s->be->be_counters.failed_resp++;
			if (objt_server(s->target)) {
				objt_server(s->target)->counters.failed_resp++;
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_HDRRSP);
			}
		abort_response:
			channel_auto_close(rep);
			rep->analysers = 0;
			txn->status = 502;
			rep->prod->flags |= SI_FL_NOLINGER;
			bi_erase(rep);
			stream_int_retnclose(rep->cons, http_error_message(s, HTTP_ERR_502));

			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_PRXCOND;
			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_H;

			return 0;
		}

		 
		else if (buffer_full(rep->buf, global.tune.maxrewrite)) {
			if (msg->err_pos < 0)
				msg->err_pos = rep->buf->i;
			goto hdr_response_bad;
		}

		 
		else if (rep->flags & CF_READ_ERROR) {
			if (msg->err_pos >= 0)
				http_capture_bad_message(&s->be->invalid_rep, s, msg, msg->msg_state, s->fe);
			else if (txn->flags & TX_NOT_FIRST)
				goto abort_keep_alive;

			s->be->be_counters.failed_resp++;
			if (objt_server(s->target)) {
				objt_server(s->target)->counters.failed_resp++;
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_READ_ERROR);
			}

			channel_auto_close(rep);
			rep->analysers = 0;
			txn->status = 502;
			rep->prod->flags |= SI_FL_NOLINGER;
			bi_erase(rep);
			stream_int_retnclose(rep->cons, http_error_message(s, HTTP_ERR_502));

			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_SRVCL;
			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_H;
			return 0;
		}

		 
		else if (rep->flags & CF_READ_TIMEOUT) {
			if (msg->err_pos >= 0)
				http_capture_bad_message(&s->be->invalid_rep, s, msg, msg->msg_state, s->fe);
			else if (txn->flags & TX_NOT_FIRST)
				goto abort_keep_alive;

			s->be->be_counters.failed_resp++;
			if (objt_server(s->target)) {
				objt_server(s->target)->counters.failed_resp++;
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_READ_TIMEOUT);
			}

			channel_auto_close(rep);
			rep->analysers = 0;
			txn->status = 504;
			rep->prod->flags |= SI_FL_NOLINGER;
			bi_erase(rep);
			stream_int_retnclose(rep->cons, http_error_message(s, HTTP_ERR_504));

			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_SRVTO;
			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_H;
			return 0;
		}

		 
		else if ((rep->flags & CF_SHUTR) && ((s->req->flags & (CF_SHUTR|CF_SHUTW)) == (CF_SHUTR|CF_SHUTW))) {
			s->fe->fe_counters.cli_aborts++;
			s->be->be_counters.cli_aborts++;
			if (objt_server(s->target))
				objt_server(s->target)->counters.cli_aborts++;

			rep->analysers = 0;
			channel_auto_close(rep);

			txn->status = 400;
			bi_erase(rep);
			stream_int_retnclose(rep->cons, http_error_message(s, HTTP_ERR_400));

			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_CLICL;
			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_H;

			 
			return 0;
		}

		 
		else if (rep->flags & CF_SHUTR) {
			if (msg->msg_state >= HTTP_MSG_RPVER || msg->err_pos >= 0)
				http_capture_bad_message(&s->be->invalid_rep, s, msg, msg->msg_state, s->fe);
			else if (txn->flags & TX_NOT_FIRST)
				goto abort_keep_alive;

			s->be->be_counters.failed_resp++;
			if (objt_server(s->target)) {
				objt_server(s->target)->counters.failed_resp++;
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_BROKEN_PIPE);
			}

			channel_auto_close(rep);
			rep->analysers = 0;
			txn->status = 502;
			rep->prod->flags |= SI_FL_NOLINGER;
			bi_erase(rep);
			stream_int_retnclose(rep->cons, http_error_message(s, HTTP_ERR_502));

			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_SRVCL;
			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_H;
			return 0;
		}

		 
		else if (rep->flags & CF_WRITE_ERROR) {
			if (msg->err_pos >= 0)
				http_capture_bad_message(&s->be->invalid_rep, s, msg, msg->msg_state, s->fe);
			else if (txn->flags & TX_NOT_FIRST)
				goto abort_keep_alive;

			s->be->be_counters.failed_resp++;
			rep->analysers = 0;
			channel_auto_close(rep);

			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_CLICL;
			if (!(s->flags & SN_FINST_MASK))
				s->flags |= SN_FINST_H;

			 
			return 0;
		}

		channel_dont_close(rep);
		rep->flags |= CF_READ_DONTWAIT;  
		return 0;
	}

	 

	if (unlikely(msg->err_pos >= 0))
		http_capture_bad_message(&s->be->invalid_rep, s, msg, msg->msg_state, s->fe);

	 
	n = rep->buf->p[msg->sl.st.c] - '0';
	if (n < 1 || n > 5)
		n = 0;
	 
	if (n == 4)
		session_inc_http_err_ctr(s);

	if (objt_server(s->target))
		objt_server(s->target)->counters.p.http.rsp[n]++;

	 
	if ((msg->sl.st.v_l == 8) &&
	    ((rep->buf->p[5] > '1') ||
	     ((rep->buf->p[5] == '1') && (rep->buf->p[7] >= '1'))))
		msg->flags |= HTTP_MSGF_VER_11;

	 
	txn->flags &= ~(TX_HDR_CONN_PRS|TX_HDR_CONN_CLO|TX_HDR_CONN_KAL|TX_HDR_CONN_UPG|TX_CON_CLO_SET|TX_CON_KAL_SET);

	 
	msg->flags &= ~HTTP_MSGF_XFER_LEN;

	txn->status = strl2ui(rep->buf->p + msg->sl.st.c, msg->sl.st.c_l);

	 
	if (objt_server(s->target)) {
		if (txn->status >= 100 && (txn->status < 500 || txn->status == 501 || txn->status == 505))
			health_adjust(objt_server(s->target), HANA_STATUS_HTTP_OK);
		else
			health_adjust(objt_server(s->target), HANA_STATUS_HTTP_STS);
	}

	 

	switch (txn->status) {
	case 100:
		 
		hdr_idx_init(&txn->hdr_idx);
		msg->next -= channel_forward(rep, msg->next);
		msg->msg_state = HTTP_MSG_RPBEFORE;
		txn->status = 0;
		s->logs.t_data = -1;  
		goto next_one;

	case 200:
	case 203:
	case 206:
	case 300:
	case 301:
	case 410:
		 
		if (likely(txn->meth != HTTP_METH_POST) &&
		    ((s->be->options & PR_O_CHK_CACHE) || (s->be->ck_opts & PR_CK_NOC)))
			txn->flags |= TX_CACHEABLE | TX_CACHE_COOK;
		break;
	default:
		break;
	}

	 
	s->logs.logwait &= ~LW_RESP;
	if (unlikely((s->logs.logwait & LW_RSPHDR) && txn->rsp.cap))
		capture_headers(rep->buf->p, &txn->hdr_idx,
				txn->rsp.cap, s->fe->rsp_cap);

	 

	 
	if (txn->meth == HTTP_METH_HEAD ||
	    (txn->status >= 100 && txn->status < 200) ||
	    txn->status == 204 || txn->status == 304) {
		msg->flags |= HTTP_MSGF_XFER_LEN;
		s->comp_algo = NULL;
		goto skip_content_length;
	}

	use_close_only = 0;
	ctx.idx = 0;
	while ((msg->flags & HTTP_MSGF_VER_11) &&
	       http_find_header2("Transfer-Encoding", 17, rep->buf->p, &txn->hdr_idx, &ctx)) {
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
	       http_find_header2("Content-Length", 14, rep->buf->p, &txn->hdr_idx, &ctx)) {
		signed long long cl;

		if (!ctx.vlen) {
			msg->err_pos = ctx.line + ctx.val - rep->buf->p;
			goto hdr_response_bad;
		}

		if (strl2llrc(ctx.line + ctx.val, ctx.vlen, &cl)) {
			msg->err_pos = ctx.line + ctx.val - rep->buf->p;
			goto hdr_response_bad;  
		}

		if (cl < 0) {
			msg->err_pos = ctx.line + ctx.val - rep->buf->p;
			goto hdr_response_bad;
		}

		if ((msg->flags & HTTP_MSGF_CNT_LEN) && (msg->chunk_len != cl)) {
			msg->err_pos = ctx.line + ctx.val - rep->buf->p;
			goto hdr_response_bad;  
		}

		msg->flags |= HTTP_MSGF_CNT_LEN | HTTP_MSGF_XFER_LEN;
		msg->body_len = msg->chunk_len = cl;
	}

	if (s->fe->comp || s->be->comp)
		select_compression_response_header(s, rep->buf);

skip_content_length:
	 

	if (unlikely((txn->meth == HTTP_METH_CONNECT && txn->status == 200) ||
		     txn->status == 101)) {
		 
		txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | TX_CON_WANT_TUN;
	}
	else if ((txn->status >= 200) && !(txn->flags & TX_HDR_CONN_PRS) &&
		 ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN ||
		  ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
		   (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL))) {
		int to_del = 0;

		 
		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL &&
		    ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
		     (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL))
			txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | TX_CON_WANT_CLO;

		 
		if (!(msg->flags & HTTP_MSGF_XFER_LEN) &&
		    (txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN)
			txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | TX_CON_WANT_CLO;

		 
		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_TUN ||
		    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_CLO) {
			to_del |= 2;  
			if (!(msg->flags & HTTP_MSGF_VER_11))
				to_del |= 1;  
		}
		else {  
			to_del |= 1;  
			if (txn->req.flags & msg->flags & HTTP_MSGF_VER_11)
				to_del |= 2;  
		}

		 
		http_parse_connection_header(txn, msg, to_del);

		 
		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL) {
			if ((txn->flags & TX_HDR_CONN_CLO) ||
			    (!(txn->flags & TX_HDR_CONN_KAL) && !(msg->flags & HTTP_MSGF_VER_11)))
				txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | TX_CON_WANT_SCL;
		}
	}

	 
	s->logs.t_data = tv_ms_elapsed(&s->logs.tv_accept, &now);

	 
	rep->analysers &= ~an_bit;
	rep->analyse_exp = TICK_ETERNITY;
	channel_auto_close(rep);
	return 1;

 abort_keep_alive:
	 
	txn->status = 0;
	rep->analysers = 0;
	s->req->analysers = 0;
	channel_auto_close(rep);
	s->logs.logwait = 0;
	s->logs.level = 0;
	s->rep->flags &= ~CF_EXPECT_MORE;  
	bi_erase(rep);
	stream_int_retnclose(rep->cons, NULL);
	return 0;
}
