int http_wait_for_response(struct stream *s, struct channel *rep, int an_bit)
{
	struct session *sess = s->sess;
	struct http_txn *txn = s->txn;
	struct http_msg *msg = &txn->rsp;
	struct hdr_ctx ctx;
	int use_close_only;
	int cur_idx;
	int n;

	DPRINTF(stderr,"[%u] %s: stream=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		rep,
		rep->rex, rep->wex,
		rep->flags,
		rep->buf->i,
		rep->analysers);

	 

 next_one:
	 
	if (buffer_not_empty(rep->buf) && msg->msg_state < HTTP_MSG_ERROR) {
		if (unlikely(!channel_is_rewritable(rep))) {
			 
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
		     msg->msg_state >= HTTP_MSG_BODY)) {
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
				http_capture_bad_message(s->be, &s->be->invalid_rep, s, msg, msg->err_state, sess->fe);

			HA_ATOMIC_ADD(&s->be->be_counters.failed_resp, 1);
			if (objt_server(s->target)) {
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.failed_resp, 1);
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_HDRRSP);
			}
		abort_response:
			channel_auto_close(rep);
			rep->analysers &= AN_RES_FLT_END;
			txn->status = 502;
			s->si[1].flags |= SI_FL_NOLINGER;
			channel_truncate(rep);
			http_reply_and_close(s, txn->status, http_error_message(s));

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_PRXCOND;
			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_H;

			return 0;
		}

		 
		else if (buffer_full(rep->buf, global.tune.maxrewrite)) {
			if (msg->err_pos < 0)
				msg->err_pos = rep->buf->i;
			goto hdr_response_bad;
		}

		 
		else if (rep->flags & CF_READ_ERROR) {
			if (msg->err_pos >= 0)
				http_capture_bad_message(s->be, &s->be->invalid_rep, s, msg, msg->err_state, sess->fe);
			else if (txn->flags & TX_NOT_FIRST)
				goto abort_keep_alive;

			HA_ATOMIC_ADD(&s->be->be_counters.failed_resp, 1);
			if (objt_server(s->target)) {
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.failed_resp, 1);
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_READ_ERROR);
			}

			channel_auto_close(rep);
			rep->analysers &= AN_RES_FLT_END;
			txn->status = 502;

			 
			if (objt_cs(s->si[1].end)) {
				struct connection *conn = objt_cs(s->si[1].end)->conn;

				if (conn->err_code == CO_ER_SSL_EARLY_FAILED)
					txn->status = 425;
			}

			s->si[1].flags |= SI_FL_NOLINGER;
			channel_truncate(rep);
			http_reply_and_close(s, txn->status, http_error_message(s));

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_SRVCL;
			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_H;
			return 0;
		}

		 
		else if (rep->flags & CF_READ_TIMEOUT) {
			if (msg->err_pos >= 0)
				http_capture_bad_message(s->be, &s->be->invalid_rep, s, msg, msg->err_state, sess->fe);

			HA_ATOMIC_ADD(&s->be->be_counters.failed_resp, 1);
			if (objt_server(s->target)) {
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.failed_resp, 1);
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_READ_TIMEOUT);
			}

			channel_auto_close(rep);
			rep->analysers &= AN_RES_FLT_END;
			txn->status = 504;
			s->si[1].flags |= SI_FL_NOLINGER;
			channel_truncate(rep);
			http_reply_and_close(s, txn->status, http_error_message(s));

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_SRVTO;
			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_H;
			return 0;
		}

		 
		else if ((rep->flags & CF_SHUTR) && ((s->req.flags & (CF_SHUTR|CF_SHUTW)) == (CF_SHUTR|CF_SHUTW))) {
			HA_ATOMIC_ADD(&sess->fe->fe_counters.cli_aborts, 1);
			HA_ATOMIC_ADD(&s->be->be_counters.cli_aborts, 1);
			if (objt_server(s->target))
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.cli_aborts, 1);

			rep->analysers &= AN_RES_FLT_END;
			channel_auto_close(rep);

			txn->status = 400;
			channel_truncate(rep);
			http_reply_and_close(s, txn->status, http_error_message(s));

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_CLICL;
			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_H;

			 
			return 0;
		}

		 
		else if (rep->flags & CF_SHUTR) {
			if (msg->msg_state >= HTTP_MSG_RPVER || msg->err_pos >= 0)
				http_capture_bad_message(s->be, &s->be->invalid_rep, s, msg, msg->err_state, sess->fe);
			else if (txn->flags & TX_NOT_FIRST)
				goto abort_keep_alive;

			HA_ATOMIC_ADD(&s->be->be_counters.failed_resp, 1);
			if (objt_server(s->target)) {
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.failed_resp, 1);
				health_adjust(objt_server(s->target), HANA_STATUS_HTTP_BROKEN_PIPE);
			}

			channel_auto_close(rep);
			rep->analysers &= AN_RES_FLT_END;
			txn->status = 502;
			s->si[1].flags |= SI_FL_NOLINGER;
			channel_truncate(rep);
			http_reply_and_close(s, txn->status, http_error_message(s));

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_SRVCL;
			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_H;
			return 0;
		}

		 
		else if (rep->flags & CF_WRITE_ERROR) {
			if (msg->err_pos >= 0)
				http_capture_bad_message(s->be, &s->be->invalid_rep, s, msg, msg->err_state, sess->fe);
			else if (txn->flags & TX_NOT_FIRST)
				goto abort_keep_alive;

			HA_ATOMIC_ADD(&s->be->be_counters.failed_resp, 1);
			rep->analysers &= AN_RES_FLT_END;
			channel_auto_close(rep);

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_CLICL;
			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_H;

			 
			return 0;
		}

		channel_dont_close(rep);
		rep->flags |= CF_READ_DONTWAIT;  
		return 0;
	}

	 

	if (unlikely(msg->err_pos >= 0))
		http_capture_bad_message(s->be, &s->be->invalid_rep, s, msg, msg->err_state, sess->fe);

	 
	n = rep->buf->p[msg->sl.st.c] - '0';
	if (n < 1 || n > 5)
		n = 0;
	 
	if (n == 4)
		stream_inc_http_err_ctr(s);

	if (objt_server(s->target))
		HA_ATOMIC_ADD(&objt_server(s->target)->counters.p.http.rsp[n], 1);

	 
	if (!(s->be->options2 & PR_O2_RSPBUG_OK)) {
		if (msg->sl.st.v_l != 8) {
			msg->err_pos = 0;
			goto hdr_response_bad;
		}

		if (rep->buf->p[4] != '/' ||
		    !isdigit((unsigned char)rep->buf->p[5]) ||
		    rep->buf->p[6] != '.' ||
		    !isdigit((unsigned char)rep->buf->p[7])) {
			msg->err_pos = 4;
			goto hdr_response_bad;
		}
	}

	 
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

	 
	if (txn->status < 200 &&
	    (txn->status == 100 || txn->status >= 102)) {
		hdr_idx_init(&txn->hdr_idx);
		msg->next -= channel_forward(rep, msg->next);
		msg->msg_state = HTTP_MSG_RPBEFORE;
		txn->status = 0;
		s->logs.t_data = -1;  
		FLT_STRM_CB(s, flt_http_reset(s, msg));
		goto next_one;
	}

	 

	switch (txn->status) {
	case 200:
	case 203:
	case 204:
	case 206:
	case 300:
	case 301:
	case 404:
	case 405:
	case 410:
	case 414:
	case 501:
		break;
	default:
		 
		txn->flags &= ~(TX_CACHEABLE | TX_CACHE_COOK);
		break;
	}

	 
	s->logs.logwait &= ~LW_RESP;
	if (unlikely((s->logs.logwait & LW_RSPHDR) && s->res_cap))
		capture_headers(rep->buf->p, &txn->hdr_idx,
				s->res_cap, sess->fe->rsp_cap);

	 

	 
	if (unlikely((txn->meth == HTTP_METH_CONNECT && txn->status == 200) ||
		     txn->status == 101)) {
		 
		txn->flags = (txn->flags & ~TX_CON_WANT_MSK) | TX_CON_WANT_TUN;
		msg->flags |= HTTP_MSGF_XFER_LEN;
		goto end;
	}

	if (txn->meth == HTTP_METH_HEAD ||
	    (txn->status >= 100 && txn->status < 200) ||
	    txn->status == 204 || txn->status == 304) {
		msg->flags |= HTTP_MSGF_XFER_LEN;
		goto skip_content_length;
	}

	use_close_only = 0;
	ctx.idx = 0;
	while (http_find_header2("Transfer-Encoding", 17, rep->buf->p, &txn->hdr_idx, &ctx)) {
		if (ctx.vlen == 7 && strncasecmp(ctx.line + ctx.val, "chunked", 7) == 0)
			msg->flags |= (HTTP_MSGF_TE_CHNK | HTTP_MSGF_XFER_LEN);
		else if (msg->flags & HTTP_MSGF_TE_CHNK) {
			 
			use_close_only = 1;
			msg->flags &= ~(HTTP_MSGF_TE_CHNK | HTTP_MSGF_XFER_LEN);
			break;
		}
	}

	 
	ctx.idx = 0;
	if (use_close_only || (msg->flags & HTTP_MSGF_TE_CHNK)) {
		while (http_find_header2("Content-Length", 14, rep->buf->p, &txn->hdr_idx, &ctx))
			http_remove_header2(msg, &txn->hdr_idx, &ctx);
	}
	else while (http_find_header2("Content-Length", 14, rep->buf->p, &txn->hdr_idx, &ctx)) {
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

 skip_content_length:
	 
	if ((txn->status >= 200) && !(txn->flags & TX_HDR_CONN_PRS) &&
	    ((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN ||
	     ((sess->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
	      (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL))) {
		int to_del = 0;

		 
		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL &&
		    ((sess->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
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

 end:
	 
	s->logs.t_data = tv_ms_elapsed(&s->logs.tv_accept, &now);

	 
	rep->analysers &= ~an_bit;
	rep->analyse_exp = TICK_ETERNITY;
	channel_auto_close(rep);
	return 1;

 abort_keep_alive:
	 
	txn->status = 0;
	rep->analysers   &= AN_RES_FLT_END;
	s->req.analysers &= AN_REQ_FLT_END;
	channel_auto_close(rep);
	s->logs.logwait = 0;
	s->logs.level = 0;
	s->res.flags &= ~CF_EXPECT_MORE;  
	channel_truncate(rep);
	http_reply_and_close(s, txn->status, NULL);
	return 0;
}
