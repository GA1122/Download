int http_process_req_common(struct session *s, struct channel *req, int an_bit, struct proxy *px)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &txn->req;
	struct redirect_rule *rule;
	struct cond_wordlist *wl;
	enum rule_result verdict;

	if (unlikely(msg->msg_state < HTTP_MSG_BODY)) {
		 
		channel_dont_connect(req);
		return 0;
	}

	DPRINTF(stderr,"[%u] %s: session=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		req,
		req->rex, req->wex,
		req->flags,
		req->buf->i,
		req->analysers);

	 
	session_inc_be_http_req_ctr(s);

	 
	if (!LIST_ISEMPTY(&px->http_req_rules)) {
		verdict = http_req_get_intercept_rule(px, &px->http_req_rules, s, txn);

		switch (verdict) {
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

	 
	if (stats_check_uri(s->rep->prod, txn, px)) {
		s->target = &http_stats_applet.obj_type;
		if (unlikely(!stream_int_register_handler(s->rep->prod, objt_applet(s->target)))) {
			txn->status = 500;
			s->logs.tv_request = now;
			stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_500));

			if (!(s->flags & SN_ERR_MASK))
				s->flags |= SN_ERR_RESOURCE;
			goto return_prx_cond;
		}

		 
		http_handle_stats(s, req);
		verdict = http_req_get_intercept_rule(px, &px->uri_auth->http_req_rules, s, txn);
		 

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

		if (txn->flags & TX_CLTARPIT)
			goto tarpit;
	}

	 
	list_for_each_entry(wl, &px->req_add, list) {
		if (wl->cond) {
			int ret = acl_exec_cond(wl->cond, px, s, txn, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);
			ret = acl_pass(ret);
			if (((struct acl_cond *)wl->cond)->pol == ACL_COND_UNLESS)
				ret = !ret;
			if (!ret)
				continue;
		}

		if (unlikely(http_header_add_tail(&txn->req, &txn->hdr_idx, wl->s) < 0))
			goto return_bad_req;
	}


	 
	if (unlikely(objt_applet(s->target) == &http_stats_applet)) {
		 
		if (s->fe == s->be)  
			s->fe->fe_counters.intercepted_req++;

		if (!(s->flags & SN_ERR_MASK))       
			s->flags |= SN_ERR_LOCAL;    
		if (!(s->flags & SN_FINST_MASK))
			s->flags |= SN_FINST_R;

		 
		if (s->fe->comp || s->be->comp)
			select_compression_request_header(s, req->buf);

		 
		req->analysers = (req->analysers & AN_REQ_HTTP_BODY) |
		                 AN_REQ_HTTP_XFER_BODY | AN_RES_WAIT_HTTP | AN_RES_HTTP_PROCESS_BE | AN_RES_HTTP_XFER_BODY;
		goto done;
	}

	 
	list_for_each_entry(rule, &px->redirect_rules, list) {
		if (rule->cond) {
			int ret;

			ret = acl_exec_cond(rule->cond, px, s, txn, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);
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
	 
	channel_erase(s->req);

	 
	channel_dont_connect(req);
	req->analysers = 0;  
	req->analysers |= AN_REQ_HTTP_TARPIT;
	req->analyse_exp = tick_add_ifset(now_ms,  s->be->timeout.tarpit);
	if (!req->analyse_exp)
		req->analyse_exp = tick_add(now_ms, 0);
	session_inc_http_err_ctr(s);
	s->fe->fe_counters.denied_req++;
	if (s->fe != s->be)
		s->be->be_counters.denied_req++;
	if (s->listener->counters)
		s->listener->counters->denied_req++;
	goto done_without_exp;

 deny:	 
	txn->flags |= TX_CLDENY;
	txn->status = 403;
	s->logs.tv_request = now;
	stream_int_retnclose(req->prod, http_error_message(s, HTTP_ERR_403));
	session_inc_http_err_ctr(s);
	s->fe->fe_counters.denied_req++;
	if (s->fe != s->be)
		s->be->be_counters.denied_req++;
	if (s->listener->counters)
		s->listener->counters->denied_req++;
	goto return_prx_cond;

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
