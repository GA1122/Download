int http_process_res_common(struct stream *s, struct channel *rep, int an_bit, struct proxy *px)
{
	struct session *sess = s->sess;
	struct http_txn *txn = s->txn;
	struct http_msg *msg = &txn->rsp;
	struct proxy *cur_proxy;
	struct cond_wordlist *wl;
	enum rule_result ret = HTTP_RULE_RES_CONT;

	DPRINTF(stderr,"[%u] %s: stream=%p b=%p, exp(r,w)=%u,%u bf=%08x bh=%d analysers=%02x\n",
		now_ms, __FUNCTION__,
		s,
		rep,
		rep->rex, rep->wex,
		rep->flags,
		rep->buf->i,
		rep->analysers);

	if (unlikely(msg->msg_state < HTTP_MSG_BODY))	 
		return 0;

	 
	if (unlikely(objt_applet(s->target) == &http_stats_applet)) {
		rep->analysers &= ~an_bit;
		rep->analyse_exp = TICK_ETERNITY;
		goto skip_filters;
	}

	 
	if (s->current_rule_list == &sess->fe->http_res_rules)
		cur_proxy = sess->fe;
	else
		cur_proxy = s->be;
	while (1) {
		struct proxy *rule_set = cur_proxy;

		 
		if (ret == HTTP_RULE_RES_CONT) {
			ret = http_res_get_intercept_rule(cur_proxy, &cur_proxy->http_res_rules, s);

			if (ret == HTTP_RULE_RES_BADREQ)
				goto return_srv_prx_502;

			if (ret == HTTP_RULE_RES_DONE) {
				rep->analysers &= ~an_bit;
				rep->analyse_exp = TICK_ETERNITY;
				return 1;
			}
		}

		 
		if (ret == HTTP_RULE_RES_YIELD) {
			channel_dont_close(rep);
			return 0;
		}

		 
		if (rule_set->rsp_exp != NULL) {
			if (apply_filters_to_response(s, rep, rule_set) < 0) {
			return_bad_resp:
				if (objt_server(s->target)) {
					HA_ATOMIC_ADD(&objt_server(s->target)->counters.failed_resp, 1);
					health_adjust(objt_server(s->target), HANA_STATUS_HTTP_RSP);
				}
				HA_ATOMIC_ADD(&s->be->be_counters.failed_resp, 1);
			return_srv_prx_502:
				rep->analysers &= AN_RES_FLT_END;
				txn->status = 502;
				s->logs.t_data = -1;  
				s->si[1].flags |= SI_FL_NOLINGER;
				channel_truncate(rep);
				http_reply_and_close(s, txn->status, http_error_message(s));
				if (!(s->flags & SF_ERR_MASK))
					s->flags |= SF_ERR_PRXCOND;
				if (!(s->flags & SF_FINST_MASK))
					s->flags |= SF_FINST_H;
				return 0;
			}
		}

		 
		if (txn->flags & TX_SVDENY) {
			if (objt_server(s->target))
				HA_ATOMIC_ADD(&objt_server(s->target)->counters.failed_secu, 1);

			HA_ATOMIC_ADD(&s->be->be_counters.denied_resp, 1);
			HA_ATOMIC_ADD(&sess->fe->fe_counters.denied_resp, 1);
			if (sess->listener->counters)
				HA_ATOMIC_ADD(&sess->listener->counters->denied_resp, 1);

			goto return_srv_prx_502;
		}

		 
		list_for_each_entry(wl, &rule_set->rsp_add, list) {
			if (txn->status < 200 && txn->status != 101)
				break;
			if (wl->cond) {
				int ret = acl_exec_cond(wl->cond, px, sess, s, SMP_OPT_DIR_RES|SMP_OPT_FINAL);
				ret = acl_pass(ret);
				if (((struct acl_cond *)wl->cond)->pol == ACL_COND_UNLESS)
					ret = !ret;
				if (!ret)
					continue;
			}
			if (unlikely(http_header_add_tail(&txn->rsp, &txn->hdr_idx, wl->s) < 0))
				goto return_bad_resp;
		}

		 
		if (cur_proxy == sess->fe)
			break;
		cur_proxy = sess->fe;
	}

	 
	rep->analysers &= ~an_bit;
	rep->analyse_exp = TICK_ETERNITY;

	 
	if (unlikely(txn->status < 200 && txn->status != 101))
		goto skip_header_mangling;

	 
	if (s->be->cookie_name || sess->fe->capture_name || (s->be->options & PR_O_CHK_CACHE))
		manage_server_side_cookies(s, rep);

	 
	if ((s->be->options & PR_O_CHK_CACHE) || (s->be->ck_opts & PR_CK_NOC))
		check_response_for_cacheability(s, rep);

	 
	if (objt_server(s->target) && (s->be->ck_opts & PR_CK_INS) &&
	    !((txn->flags & TX_SCK_FOUND) && (s->be->ck_opts & PR_CK_PSV)) &&
	    (!(s->flags & SF_DIRECT) ||
	     ((s->be->cookie_maxidle || txn->cookie_last_date) &&
	      (!txn->cookie_last_date || (txn->cookie_last_date - date.tv_sec) < 0)) ||
	     (s->be->cookie_maxlife && !txn->cookie_first_date) ||   
	     (!s->be->cookie_maxlife && txn->cookie_first_date)) &&  
	    (!(s->be->ck_opts & PR_CK_POST) || (txn->meth == HTTP_METH_POST)) &&
	    !(s->flags & SF_IGNORE_PRST)) {
		 
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
		if (objt_server(s->target)->cookie && (s->flags & SF_DIRECT))
			 
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
			HA_ATOMIC_ADD(&objt_server(s->target)->counters.failed_secu, 1);

		HA_ATOMIC_ADD(&s->be->be_counters.denied_resp, 1);
		HA_ATOMIC_ADD(&sess->fe->fe_counters.denied_resp, 1);
		if (sess->listener->counters)
			HA_ATOMIC_ADD(&sess->listener->counters->denied_resp, 1);

		ha_alert("Blocking cacheable cookie in response from instance %s, server %s.\n",
			 s->be->id, objt_server(s->target) ? objt_server(s->target)->id : "<dispatch>");
		send_log(s->be, LOG_ALERT,
			 "Blocking cacheable cookie in response from instance %s, server %s.\n",
			 s->be->id, objt_server(s->target) ? objt_server(s->target)->id : "<dispatch>");
		goto return_srv_prx_502;
	}

 skip_filters:
	 
	if ((txn->status != 101) && !(txn->flags & TX_HDR_CONN_UPG) &&
	    (((txn->flags & TX_CON_WANT_MSK) != TX_CON_WANT_TUN) ||
	     ((sess->fe->options & PR_O_HTTP_MODE) == PR_O_HTTP_PCL ||
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
	 
	rep->analysers &= ~AN_RES_FLT_XFER_DATA;
	rep->analysers |= AN_RES_HTTP_XFER_BODY;

	 
	if (!LIST_ISEMPTY(&sess->fe->logformat) && !(s->logs.logwait & LW_BYTES)) {
		s->logs.t_close = s->logs.t_data;  
		s->logs.bytes_out = txn->rsp.eoh;
		s->do_log(s);
		s->logs.bytes_out = 0;
	}
	return 1;
}
