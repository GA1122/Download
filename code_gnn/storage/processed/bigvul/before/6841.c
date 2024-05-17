int http_process_request(struct stream *s, struct channel *req, int an_bit)
{
	struct session *sess = s->sess;
	struct http_txn *txn = s->txn;
	struct http_msg *msg = &txn->req;
	struct connection *cli_conn = objt_conn(strm_sess(s)->origin);

	if (unlikely(msg->msg_state < HTTP_MSG_BODY)) {
		 
		channel_dont_connect(req);
		return 0;
	}

	DPRINTF(stderr,"[%u] %s: stream=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		req,
		req->rex, req->wex,
		req->flags,
		req->buf->i,
		req->analysers);

	 

	 
	if ((s->be->options & PR_O_HTTP_PROXY) && !(s->flags & SF_ADDR_SET)) {
		struct connection *conn;
		char *path;

		 
		if (unlikely((conn = cs_conn(si_alloc_cs(&s->si[1], NULL))) == NULL)) {
			txn->req.err_state = txn->req.msg_state;
			txn->req.msg_state = HTTP_MSG_ERROR;
			txn->status = 500;
			req->analysers &= AN_REQ_FLT_END;
			http_reply_and_close(s, txn->status, http_error_message(s));

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_RESOURCE;
			if (!(s->flags & SF_FINST_MASK))
				s->flags |= SF_FINST_R;

			return 0;
		}

		path = http_get_path(txn);
		if (url2sa(req->buf->p + msg->sl.rq.u,
			   path ? path - (req->buf->p + msg->sl.rq.u) : msg->sl.rq.u_l,
			   &conn->addr.to, NULL) == -1)
			goto return_bad_req;

		 
		if (path) {
			char *cur_ptr = req->buf->p;
			char *cur_end = cur_ptr + txn->req.sl.rq.l;
			int delta;

			delta = buffer_replace2(req->buf, req->buf->p + msg->sl.rq.u, path, NULL, 0);
			http_msg_move_end(&txn->req, delta);
			cur_end += delta;
			if (http_parse_reqline(&txn->req, HTTP_MSG_RQMETH,  cur_ptr, cur_end + 1, NULL, NULL) == NULL)
				goto return_bad_req;
		}
		else {
			char *cur_ptr = req->buf->p;
			char *cur_end = cur_ptr + txn->req.sl.rq.l;
			int delta;

			delta = buffer_replace2(req->buf, req->buf->p + msg->sl.rq.u,
						req->buf->p + msg->sl.rq.u + msg->sl.rq.u_l, "/", 1);
			http_msg_move_end(&txn->req, delta);
			cur_end += delta;
			if (http_parse_reqline(&txn->req, HTTP_MSG_RQMETH,  cur_ptr, cur_end + 1, NULL, NULL) == NULL)
				goto return_bad_req;
		}
	}

	 
	if (s->be->cookie_name || sess->fe->capture_name)
		manage_client_side_cookies(s, req);

	 

	if (!LIST_ISEMPTY(&sess->fe->format_unique_id) && !s->unique_id) {
		if ((s->unique_id = pool_alloc(pool_head_uniqueid)) == NULL)
			goto return_bad_req;
		s->unique_id[0] = '\0';
		build_logline(s, s->unique_id, UNIQUEID_LEN, &sess->fe->format_unique_id);
	}

	if (sess->fe->header_unique_id && s->unique_id) {
		chunk_printf(&trash, "%s: %s", sess->fe->header_unique_id, s->unique_id);
		if (trash.len < 0)
			goto return_bad_req;
		if (unlikely(http_header_add_tail2(&txn->req, &txn->hdr_idx, trash.str, trash.len) < 0))
		   goto return_bad_req;
	}

	 
	if ((sess->fe->options | s->be->options) & PR_O_FWDFOR) {
		struct hdr_ctx ctx = { .idx = 0 };
		if (!((sess->fe->options | s->be->options) & PR_O_FF_ALWAYS) &&
			http_find_header2(s->be->fwdfor_hdr_len ? s->be->fwdfor_hdr_name : sess->fe->fwdfor_hdr_name,
			                  s->be->fwdfor_hdr_len ? s->be->fwdfor_hdr_len : sess->fe->fwdfor_hdr_len,
			                  req->buf->p, &txn->hdr_idx, &ctx)) {
			 
		}
		else if (cli_conn && cli_conn->addr.from.ss_family == AF_INET) {
			 
			if ((!sess->fe->except_mask.s_addr ||
			     (((struct sockaddr_in *)&cli_conn->addr.from)->sin_addr.s_addr & sess->fe->except_mask.s_addr)
			     != sess->fe->except_net.s_addr) &&
			    (!s->be->except_mask.s_addr ||
			     (((struct sockaddr_in *)&cli_conn->addr.from)->sin_addr.s_addr & s->be->except_mask.s_addr)
			     != s->be->except_net.s_addr)) {
				int len;
				unsigned char *pn;
				pn = (unsigned char *)&((struct sockaddr_in *)&cli_conn->addr.from)->sin_addr;

				 
				if (s->be->fwdfor_hdr_len) {
					len = s->be->fwdfor_hdr_len;
					memcpy(trash.str, s->be->fwdfor_hdr_name, len);
				} else {
					len = sess->fe->fwdfor_hdr_len;
					memcpy(trash.str, sess->fe->fwdfor_hdr_name, len);
				}
				len += snprintf(trash.str + len, trash.size - len, ": %d.%d.%d.%d", pn[0], pn[1], pn[2], pn[3]);

				if (unlikely(http_header_add_tail2(&txn->req, &txn->hdr_idx, trash.str, len) < 0))
					goto return_bad_req;
			}
		}
		else if (cli_conn && cli_conn->addr.from.ss_family == AF_INET6) {
			 
			int len;
			char pn[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6,
				  (const void *)&((struct sockaddr_in6 *)(&cli_conn->addr.from))->sin6_addr,
				  pn, sizeof(pn));

			 
			if (s->be->fwdfor_hdr_len) {
				len = s->be->fwdfor_hdr_len;
				memcpy(trash.str, s->be->fwdfor_hdr_name, len);
			} else {
				len = sess->fe->fwdfor_hdr_len;
				memcpy(trash.str, sess->fe->fwdfor_hdr_name, len);
			}
			len += snprintf(trash.str + len, trash.size - len, ": %s", pn);

			if (unlikely(http_header_add_tail2(&txn->req, &txn->hdr_idx, trash.str, len) < 0))
				goto return_bad_req;
		}
	}

	 
	if ((sess->fe->options | s->be->options) & PR_O_ORGTO) {

		 
		if (cli_conn && cli_conn->addr.from.ss_family == AF_INET) {
			 
			conn_get_to_addr(cli_conn);

			if (cli_conn->addr.to.ss_family == AF_INET &&
			    ((!sess->fe->except_mask_to.s_addr ||
			      (((struct sockaddr_in *)&cli_conn->addr.to)->sin_addr.s_addr & sess->fe->except_mask_to.s_addr)
			      != sess->fe->except_to.s_addr) &&
			     (!s->be->except_mask_to.s_addr ||
			      (((struct sockaddr_in *)&cli_conn->addr.to)->sin_addr.s_addr & s->be->except_mask_to.s_addr)
			      != s->be->except_to.s_addr))) {
				int len;
				unsigned char *pn;
				pn = (unsigned char *)&((struct sockaddr_in *)&cli_conn->addr.to)->sin_addr;

				 
				if (s->be->orgto_hdr_len) {
					len = s->be->orgto_hdr_len;
					memcpy(trash.str, s->be->orgto_hdr_name, len);
				} else {
					len = sess->fe->orgto_hdr_len;
					memcpy(trash.str, sess->fe->orgto_hdr_name, len);
				}
				len += snprintf(trash.str + len, trash.size - len, ": %d.%d.%d.%d", pn[0], pn[1], pn[2], pn[3]);

				if (unlikely(http_header_add_tail2(&txn->req, &txn->hdr_idx, trash.str, len) < 0))
					goto return_bad_req;
			}
		}
	}

	 
	if (!(txn->flags & TX_HDR_CONN_UPG) &&
	    (((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN) ||
	     ((sess->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
	      (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL))) {
		unsigned int want_flags = 0;

		if (msg->flags & HTTP_MSGF_VER_11) {
			if (((txn->flags & TX_CON_WANT_MSK) >= TX_CON_WANT_SCL ||
			     ((sess->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
			      (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL)) &&
			    !((sess->fe->options2|s->be->options2) & PR_O2_FAKE_KA))
				want_flags |= TX_CON_CLO_SET;
		} else {
			if (((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL &&
			     ((sess->fe->options & PR_O_HTTP_MODE) != PR_O_HTTP_PCL &&
			      (s->be->options & PR_O_HTTP_MODE) != PR_O_HTTP_PCL)) ||
			    ((sess->fe->options2|s->be->options2) & PR_O2_FAKE_KA))
				want_flags |= TX_CON_KAL_SET;
		}

		if (want_flags != (txn->flags & (TX_CON_CLO_SET|TX_CON_KAL_SET)))
			http_change_connection_header(txn, msg, want_flags);
	}


	 
	if (!(s->flags & (SF_ASSIGNED|SF_DIRECT)) &&
	    s->txn->meth == HTTP_METH_POST && s->be->url_param_name != NULL &&
	    (msg->flags & (HTTP_MSGF_CNT_LEN|HTTP_MSGF_TE_CHNK))) {
		channel_dont_connect(req);
		req->analysers |= AN_REQ_HTTP_BODY;
	}

	req->analysers &= ~AN_REQ_FLT_XFER_DATA;
	req->analysers |= AN_REQ_HTTP_XFER_BODY;
#ifdef TCP_QUICKACK
	 
	if ((sess->listener->options & LI_O_NOQUICKACK) &&
	    cli_conn && conn_ctrl_ready(cli_conn) &&
	    ((msg->flags & HTTP_MSGF_TE_CHNK) ||
	     (msg->body_len > req->buf->i - txn->req.eoh - 2)))
		setsockopt(cli_conn->handle.fd, IPPROTO_TCP, TCP_QUICKACK, &one, sizeof(one));
#endif

	 
	req->analyse_exp = TICK_ETERNITY;
	req->analysers &= ~an_bit;

	s->logs.tv_request = now;
	 
	return 1;

 return_bad_req:  
	if (unlikely(msg->msg_state == HTTP_MSG_ERROR) || msg->err_pos >= 0) {
		 
		http_capture_bad_message(sess->fe, &sess->fe->invalid_req, s, msg, msg->err_state, sess->fe);
	}

	txn->req.err_state = txn->req.msg_state;
	txn->req.msg_state = HTTP_MSG_ERROR;
	txn->status = 400;
	req->analysers &= AN_REQ_FLT_END;
	http_reply_and_close(s, txn->status, http_error_message(s));

	HA_ATOMIC_ADD(&sess->fe->fe_counters.failed_req, 1);
	if (sess->listener->counters)
		HA_ATOMIC_ADD(&sess->listener->counters->failed_req, 1);

	if (!(s->flags & SF_ERR_MASK))
		s->flags |= SF_ERR_PRXCOND;
	if (!(s->flags & SF_FINST_MASK))
		s->flags |= SF_FINST_R;
	return 0;
}
