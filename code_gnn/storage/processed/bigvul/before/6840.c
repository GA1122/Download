int http_process_req_common(struct stream *s, struct channel *req, int an_bit, struct proxy *px)
{
	struct session *sess = s->sess;
	struct http_txn *txn = s->txn;
	struct http_msg *msg = &txn->req;
	struct redirect_rule *rule;
	struct cond_wordlist *wl;
	enum rule_result verdict;
	int deny_status = HTTP_ERR_403;
	struct connection *conn = objt_conn(sess->origin);

	if (unlikely(msg->msg_state < HTTP_MSG_BODY)) {
		 
		goto return_prx_yield;
	}

	DPRINTF(stderr,"[%u] %s: stream=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		req,
		req->rex, req->wex,
		req->flags,
		req->buf->i,
		req->analysers);

	 
	stream_inc_be_http_req_ctr(s);

	 
	if (!LIST_ISEMPTY(&px->http_req_rules)) {
		verdict = http_req_get_intercept_rule(px, &px->http_req_rules, s, &deny_status);

		switch (verdict) {
		case HTTP_RULE_RES_YIELD:  
			goto return_prx_yield;

		case HTTP_RULE_RES_CONT:
		case HTTP_RULE_RES_STOP:  
			break;

		case HTTP_RULE_RES_DENY:  
			if (txn->flags & TX_CLTARPIT)
				goto tarpit;
			goto deny;

		case HTTP_RULE_RES_ABRT:  
			goto return_prx_cond;

		case HTTP_RULE_RES_DONE:  
			goto done;

		case HTTP_RULE_RES_BADREQ:  
			goto return_bad_req;
		}
	}

	if (conn && conn->flags & CO_FL_EARLY_DATA) {
		struct hdr_ctx ctx;

		ctx.idx = 0;
		if (!http_find_header2("Early-Data", strlen("Early-Data"),
		    s->req.buf->p, &txn->hdr_idx, &ctx)) {
			if (unlikely(http_header_add_tail2(&txn->req,
			    &txn->hdr_idx, "Early-Data: 1",
			    strlen("Early-Data: 1"))) < 0) {
				goto return_bad_req;
			 }
		}

	}

	 
	if (stats_check_uri(&s->si[1], txn, px)) {
		s->target = &http_stats_applet.obj_type;
		if (unlikely(!stream_int_register_handler(&s->si[1], objt_applet(s->target)))) {
			txn->status = 500;
			s->logs.tv_request = now;
			http_reply_and_close(s, txn->status, http_error_message(s));

			if (!(s->flags & SF_ERR_MASK))
				s->flags |= SF_ERR_RESOURCE;
			goto return_prx_cond;
		}

		 
		http_handle_stats(s, req);
		verdict = http_req_get_intercept_rule(px, &px->uri_auth->http_req_rules, s, &deny_status);
		 

		if (verdict == HTTP_RULE_RES_DENY)  
			goto deny;

		if (verdict == HTTP_RULE_RES_ABRT)  
			goto return_prx_cond;
	}

	 
	if (px->req_exp != NULL) {
		if (apply_filters_to_request(s, req, px) < 0)
			goto return_bad_req;

		if (txn->flags & TX_CLDENY)
			goto deny;

		if (txn->flags & TX_CLTARPIT) {
			deny_status = HTTP_ERR_500;
			goto tarpit;
		}
	}

	 
	list_for_each_entry(wl, &px->req_add, list) {
		if (wl->cond) {
			int ret = acl_exec_cond(wl->cond, px, sess, s, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);
			ret = acl_pass(ret);
			if (((struct acl_cond *)wl->cond)->pol == ACL_COND_UNLESS)
				ret = !ret;
			if (!ret)
				continue;
		}

		if (unlikely(http_header_add_tail(&txn->req, &txn->hdr_idx, wl->s) < 0))
			goto return_bad_req;
	}


	 
	if (unlikely(objt_applet(s->target) == &http_stats_applet) ||
	    unlikely(objt_applet(s->target) == &http_cache_applet)) {
		 
		if (sess->fe == s->be)  
			HA_ATOMIC_ADD(&sess->fe->fe_counters.intercepted_req, 1);

		if (!(s->flags & SF_ERR_MASK))       
			s->flags |= SF_ERR_LOCAL;    
		if (!(s->flags & SF_FINST_MASK))
			s->flags |= SF_FINST_R;

		 
		req->analysers &= (AN_REQ_HTTP_BODY | AN_REQ_FLT_HTTP_HDRS | AN_REQ_FLT_END);
		req->analysers &= ~AN_REQ_FLT_XFER_DATA;
		req->analysers |= AN_REQ_HTTP_XFER_BODY;
		goto done;
	}

	 
	list_for_each_entry(rule, &px->redirect_rules, list) {
		if (rule->cond) {
			int ret;

			ret = acl_exec_cond(rule->cond, px, sess, s, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);
			ret = acl_pass(ret);
			if (rule->cond->pol == ACL_COND_UNLESS)
				ret = !ret;
			if (!ret)
				continue;
		}
		if (!http_apply_redirect_rule(rule, s, txn))
			goto return_bad_req;
		goto done;
	}

	 
	req->flags |= CF_SEND_DONTWAIT;

 done:	 
	req->analyse_exp = TICK_ETERNITY;
 done_without_exp:  
	req->analysers &= ~an_bit;
	return 1;

 tarpit:
	 
	if (s->be->cookie_name || sess->fe->capture_name)
		manage_client_side_cookies(s, req);

	 
	channel_erase(&s->req);

	 
	channel_dont_connect(req);

	txn->status = http_err_codes[deny_status];

	req->analysers &= AN_REQ_FLT_END;  
	req->analysers |= AN_REQ_HTTP_TARPIT;
	req->analyse_exp = tick_add_ifset(now_ms,  s->be->timeout.tarpit);
	if (!req->analyse_exp)
		req->analyse_exp = tick_add(now_ms, 0);
	stream_inc_http_err_ctr(s);
	HA_ATOMIC_ADD(&sess->fe->fe_counters.denied_req, 1);
	if (sess->fe != s->be)
		HA_ATOMIC_ADD(&s->be->be_counters.denied_req, 1);
	if (sess->listener->counters)
		HA_ATOMIC_ADD(&sess->listener->counters->denied_req, 1);
	goto done_without_exp;

 deny:	 

	 
	if (s->be->cookie_name || sess->fe->capture_name)
		manage_client_side_cookies(s, req);

	txn->flags |= TX_CLDENY;
	txn->status = http_err_codes[deny_status];
	s->logs.tv_request = now;
	http_reply_and_close(s, txn->status, http_error_message(s));
	stream_inc_http_err_ctr(s);
	HA_ATOMIC_ADD(&sess->fe->fe_counters.denied_req, 1);
	if (sess->fe != s->be)
		HA_ATOMIC_ADD(&s->be->be_counters.denied_req, 1);
	if (sess->listener->counters)
		HA_ATOMIC_ADD(&sess->listener->counters->denied_req, 1);
	goto return_prx_cond;

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

 return_prx_yield:
	channel_dont_connect(req);
	return 0;
}
