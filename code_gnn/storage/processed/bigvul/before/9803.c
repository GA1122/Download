int http_process_res_common(struct session *s, struct channel *rep, int an_bit, struct proxy *px)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &txn->rsp;
	struct proxy *cur_proxy;
	struct cond_wordlist *wl;
	struct http_res_rule *http_res_last_rule = NULL;

	DPRINTF(stderr,"[%u] %s: session=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		rep,
		rep->rex, rep->wex,
		rep->flags,
		rep->buf->i,
		rep->analysers);

	if (unlikely(msg->msg_state < HTTP_MSG_BODY))	 
		return 0;

	rep->analysers &= ~an_bit;
	rep->analyse_exp = TICK_ETERNITY;

	 
	if (unlikely(objt_applet(s->target) == &http_stats_applet))
		goto skip_filters;

	 

	cur_proxy = s->be;
	while (1) {
		struct proxy *rule_set = cur_proxy;

		 
		if (!http_res_last_rule)
			http_res_last_rule = http_res_get_intercept_rule(cur_proxy, &cur_proxy->http_res_rules, s, txn);

		 
		if (rule_set->rsp_exp != NULL) {
			if (apply_filters_to_response(s, rep, rule_set) < 0) {
			return_bad_resp:
				if (objt_server(s->target)) {
					objt_server(s->target)->counters.failed_resp++;
					health_adjust(objt_server(s->target), HANA_STATUS_HTTP_RSP);
				}
				s->be->be_counters.failed_resp++;
			return_srv_prx_502:
				rep->analysers = 0;
				txn->status = 502;
				s->logs.t_data = -1;  
				rep->prod->flags |= SI_FL_NOLINGER;
				bi_erase(rep);
				stream_int_retnclose(rep->cons, http_error_message(s, HTTP_ERR_502));
				if (!(s->flags & SN_ERR_MASK))
					s->flags |= SN_ERR_PRXCOND;
				if (!(s->flags & SN_FINST_MASK))
					s->flags |= SN_FINST_H;
				return 0;
			}
		}

		 
		if (txn->flags & TX_SVDENY) {
			if (objt_server(s->target))
				objt_server(s->target)->counters.failed_secu++;

			s->be->be_counters.denied_resp++;
			s->fe->fe_counters.denied_resp++;
			if (s->listener->counters)
				s->listener->counters->denied_resp++;

			goto return_srv_prx_502;
		}

		 
		list_for_each_entry(wl, &rule_set->rsp_add, list) {
			if (txn->status < 200)
				break;
			if (wl->cond) {
				int ret = acl_exec_cond(wl->cond, px, s, txn, SMP_OPT_DIR_RES|SMP_OPT_FINAL);
				ret = acl_pass(ret);
				if (((struct acl_cond *)wl->cond)->pol == ACL_COND_UNLESS)
					ret = !ret;
				if (!ret)
					continue;
			}
			if (unlikely(http_header_add_tail(&txn->rsp, &txn->hdr_idx, wl->s) < 0))
				goto return_bad_resp;
		}

		 
		if (cur_proxy == s->fe)
			break;
		cur_proxy = s->fe;
	}

	 
	if (unlikely(txn->status < 200))
		goto skip_header_mangling;

	 
	if (s->be->cookie_name || s->be->appsession_name || s->fe->capture_name ||
	    (s->be->options & PR_O_CHK_CACHE))
		manage_server_side_cookies(s, rep);

	 
	if ((s->be->options & PR_O_CHK_CACHE) || (s->be->ck_opts & PR_CK_NOC))
		check_response_for_cacheability(s, rep);

	 
	if (objt_server(s->target) && (s->be->ck_opts & PR_CK_INS) &&
	    !((txn->flags & TX_SCK_FOUND) && (s->be->ck_opts & PR_CK_PSV)) &&
	    (!(s->flags & SN_DIRECT) ||
	     ((s->be->cookie_maxidle || txn->cookie_last_date) &&
	      (!txn->cookie_last_date || (txn->cookie_last_date - date.tv_sec) < 0)) ||
	     (s->be->cookie_maxlife && !txn->cookie_first_date) ||   
	     (!s->be->cookie_maxlife && txn->cookie_first_date)) &&  
	    (!(s->be->ck_opts & PR_CK_POST) || (txn->meth == HTTP_METH_POST)) &&
	    !(s->flags & SN_IGNORE_PRST)) {
		 
		if (!objt_server(s->target)->cookie) {
			chunk_printf(&trash,
				     "Set-Cookie: %s=; Expires=Thu, 01-Jan-1970 00:00:01 GMT; path=/",
				     s->be->cookie_name);
		}
		else {
			chunk_printf(&trash, "Set-Cookie: %s=%s", s->be->cookie_name, objt_server(s->target)->cookie);

			if (s->be->cookie_maxidle || s->be->cookie_maxlife) {
				 
				trash.str[trash.len++] = COOKIE_DELIM_DATE;
				s30tob64((date.tv_sec+3) >> 2, trash.str + trash.len);
				trash.len += 5;

				if (s->be->cookie_maxlife) {
					 
					trash.str[trash.len++] = COOKIE_DELIM_DATE;
					s30tob64(txn->cookie_first_date ?
						 txn->cookie_first_date >> 2 :
						 (date.tv_sec+3) >> 2, trash.str + trash.len);
					trash.len += 5;
				}
			}
			chunk_appendf(&trash, "; path=/");
		}

		if (s->be->cookie_domain)
			chunk_appendf(&trash, "; domain=%s", s->be->cookie_domain);

		if (s->be->ck_opts & PR_CK_HTTPONLY)
			chunk_appendf(&trash, "; HttpOnly");

		if (s->be->ck_opts & PR_CK_SECURE)
			chunk_appendf(&trash, "; Secure");

		if (unlikely(http_header_add_tail2(&txn->rsp, &txn->hdr_idx, trash.str, trash.len) < 0))
			goto return_bad_resp;

		txn->flags &= ~TX_SCK_MASK;
		if (objt_server(s->target)->cookie && (s->flags & SN_DIRECT))
			 
			txn->flags |= TX_SCK_UPDATED;
		else
			txn->flags |= TX_SCK_INSERTED;

		 
		if ((s->be->ck_opts & PR_CK_NOC) && (txn->flags & TX_CACHEABLE)) {

			txn->flags &= ~TX_CACHEABLE & ~TX_CACHE_COOK;

			if (unlikely(http_header_add_tail2(&txn->rsp, &txn->hdr_idx,
			                                   "Cache-control: private", 22) < 0))
				goto return_bad_resp;
		}
	}

	 
	if (((txn->flags & (TX_CACHEABLE | TX_CACHE_COOK | TX_SCK_PRESENT)) ==
	     (TX_CACHEABLE | TX_CACHE_COOK | TX_SCK_PRESENT)) &&
	    (s->be->options & PR_O_CHK_CACHE)) {
		 
		if (objt_server(s->target))
			objt_server(s->target)->counters.failed_secu++;

		s->be->be_counters.denied_resp++;
		s->fe->fe_counters.denied_resp++;
		if (s->listener->counters)
			s->listener->counters->denied_resp++;

		Alert("Blocking cacheable cookie in response from instance %s, server %s.\n",
		      s->be->id, objt_server(s->target) ? objt_server(s->target)->id : "<dispatch>");
		send_log(s->be, LOG_ALERT,
			 "Blocking cacheable cookie in response from instance %s, server %s.\n",
			 s->be->id, objt_server(s->target) ? objt_server(s->target)->id : "<dispatch>");
		goto return_srv_prx_502;
	}

 skip_filters:
	 
	if (!(txn->flags & TX_HDR_CONN_UPG) &&
	    (((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN) ||
	     ((s->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
	      (s->be->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL))) {
		unsigned int want_flags = 0;

		if ((txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_KAL ||
		    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_SCL) {
			 
			if (!(txn->req.flags & msg->flags & HTTP_MSGF_VER_11))
				want_flags |= TX_CON_KAL_SET;
		}
		else {
			 
			if (msg->flags & HTTP_MSGF_VER_11)
				want_flags |= TX_CON_CLO_SET;
		}

		if (want_flags != (txn->flags & (TX_CON_CLO_SET|TX_CON_KAL_SET)))
			http_change_connection_header(txn, msg, want_flags);
	}

 skip_header_mangling:
	if ((msg->flags & HTTP_MSGF_XFER_LEN) ||
	    (txn->flags & TX_CON_WANT_MSK) == TX_CON_WANT_TUN)
		rep->analysers |= AN_RES_HTTP_XFER_BODY;

	 
	if (!LIST_ISEMPTY(&s->fe->logformat) && !(s->logs.logwait & LW_BYTES)) {
		s->logs.t_close = s->logs.t_data;  
		s->logs.bytes_out = txn->rsp.eoh;
		s->do_log(s);
		s->logs.bytes_out = 0;
	}
	return 1;
}
