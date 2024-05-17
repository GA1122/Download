void manage_client_side_cookies(struct stream *s, struct channel *req)
{
	struct http_txn *txn = s->txn;
	struct session *sess = s->sess;
	int preserve_hdr;
	int cur_idx, old_idx;
	char *hdr_beg, *hdr_end, *hdr_next, *del_from;
	char *prev, *att_beg, *att_end, *equal, *val_beg, *val_end, *next;

	 
	old_idx = 0;
	hdr_next = req->buf->p + hdr_idx_first_pos(&txn->hdr_idx);

	while ((cur_idx = txn->hdr_idx.v[old_idx].next)) {
		struct hdr_idx_elem *cur_hdr;
		int val;

		cur_hdr  = &txn->hdr_idx.v[cur_idx];
		hdr_beg  = hdr_next;
		hdr_end  = hdr_beg + cur_hdr->len;
		hdr_next = hdr_end + cur_hdr->cr + 1;

		 

		val = http_header_match2(hdr_beg, hdr_end, "Cookie", 6);
		if (!val) {
			old_idx = cur_idx;
			continue;
		}

		del_from = NULL;   
		preserve_hdr = 0;  

		 

		for (prev = hdr_beg + 6; prev < hdr_end; prev = next) {
			 

			 
			att_beg = prev + 1;
			while (att_beg < hdr_end && HTTP_IS_SPHT(*att_beg))
				att_beg++;

			 
			equal = att_end = att_beg;

			while (equal < hdr_end) {
				if (*equal == '=' || *equal == ',' || *equal == ';')
					break;
				if (HTTP_IS_SPHT(*equal++))
					continue;
				att_end = equal;
			}

			 

			 
			if (equal < hdr_end && *equal == '=') {
				 
				val_beg = equal + 1;
				while (val_beg < hdr_end && HTTP_IS_SPHT(*val_beg))
					val_beg++;

				 
				next = find_cookie_value_end(val_beg, hdr_end);

				 
				val_end = next;
				while (val_end > val_beg && HTTP_IS_SPHT(*(val_end - 1)))
					val_end--;
			} else {
				val_beg = val_end = next = equal;
			}

			 
			if (*att_beg == '$')
				continue;

			 
			if (equal == next) {
				 
				preserve_hdr = 1;
				if (del_from != NULL) {
					int delta = del_hdr_value(req->buf, &del_from, prev);
					val_end  += delta;
					next     += delta;
					hdr_end  += delta;
					hdr_next += delta;
					cur_hdr->len += delta;
					http_msg_move_end(&txn->req, delta);
					prev     = del_from;
					del_from = NULL;
				}
				continue;
			}

			 
			if (unlikely(att_end != equal || val_beg > equal + 1)) {
				int stripped_before = 0;
				int stripped_after = 0;

				if (att_end != equal) {
					stripped_before = buffer_replace2(req->buf, att_end, equal, NULL, 0);
					equal   += stripped_before;
					val_beg += stripped_before;
				}

				if (val_beg > equal + 1) {
					stripped_after = buffer_replace2(req->buf, equal + 1, val_beg, NULL, 0);
					val_beg += stripped_after;
					stripped_before += stripped_after;
				}

				val_end      += stripped_before;
				next         += stripped_before;
				hdr_end      += stripped_before;
				hdr_next     += stripped_before;
				cur_hdr->len += stripped_before;
				http_msg_move_end(&txn->req, stripped_before);
			}
			 

			 
			if (sess->fe->capture_name != NULL && txn->cli_cookie == NULL &&
			    (val_end - att_beg >= sess->fe->capture_namelen) &&
			    memcmp(att_beg, sess->fe->capture_name, sess->fe->capture_namelen) == 0) {
				int log_len = val_end - att_beg;

				if ((txn->cli_cookie = pool_alloc(pool_head_capture)) == NULL) {
					ha_alert("HTTP logging : out of memory.\n");
				} else {
					if (log_len > sess->fe->capture_len)
						log_len = sess->fe->capture_len;
					memcpy(txn->cli_cookie, att_beg, log_len);
					txn->cli_cookie[log_len] = 0;
				}
			}

			 
			if ((att_end - att_beg == s->be->cookie_len) && (s->be->cookie_name != NULL) &&
			    (memcmp(att_beg, s->be->cookie_name, att_end - att_beg) == 0)) {
				struct server *srv = s->be->srv;
				char *delim;

				 

				if (s->be->ck_opts & PR_CK_PFX) {
					for (delim = val_beg; delim < val_end; delim++)
						if (*delim == COOKIE_DELIM)
							break;
				} else {
					char *vbar1;
					delim = val_end;
					 
					vbar1 = memchr(val_beg, COOKIE_DELIM_DATE, val_end - val_beg);
					if (vbar1) {
						 
						int val;
						delim = vbar1++;
						if (val_end - vbar1 >= 5) {
							val = b64tos30(vbar1);
							if (val > 0)
								txn->cookie_last_date = val << 2;
						}
						 
						vbar1 = memchr(vbar1, COOKIE_DELIM_DATE, val_end - vbar1);
						if (vbar1 && (val_end - vbar1 > 5)) {
							val = b64tos30(vbar1 + 1);
							if (val > 0)
								txn->cookie_first_date = val << 2;
						}
					}
				}

				 
				if (txn->cookie_first_date && s->be->cookie_maxlife &&
				    (((signed)(date.tv_sec - txn->cookie_first_date) > (signed)s->be->cookie_maxlife) ||
				     ((signed)(txn->cookie_first_date - date.tv_sec) > 86400))) {
					txn->flags &= ~TX_CK_MASK;
					txn->flags |= TX_CK_OLD;
					delim = val_beg;  
					txn->cookie_first_date = 0;
					txn->cookie_last_date = 0;
				}
				else if (txn->cookie_last_date && s->be->cookie_maxidle &&
					 (((signed)(date.tv_sec - txn->cookie_last_date) > (signed)s->be->cookie_maxidle) ||
					  ((signed)(txn->cookie_last_date - date.tv_sec) > 86400))) {
					txn->flags &= ~TX_CK_MASK;
					txn->flags |= TX_CK_EXPIRED;
					delim = val_beg;  
					txn->cookie_first_date = 0;
					txn->cookie_last_date = 0;
				}

				 
				if ((delim == val_beg) || (s->flags & (SF_IGNORE_PRST | SF_ASSIGNED)))
					srv = NULL;

				while (srv) {
					if (srv->cookie && (srv->cklen == delim - val_beg) &&
					    !memcmp(val_beg, srv->cookie, delim - val_beg)) {
						if ((srv->cur_state != SRV_ST_STOPPED) ||
						    (s->be->options & PR_O_PERSIST) ||
						    (s->flags & SF_FORCE_PRST)) {
							 
							txn->flags &= ~TX_CK_MASK;
							txn->flags |= (srv->cur_state != SRV_ST_STOPPED) ? TX_CK_VALID : TX_CK_DOWN;
							s->flags |= SF_DIRECT | SF_ASSIGNED;
							s->target = &srv->obj_type;
							break;
						} else {
							 
							txn->flags &= ~TX_CK_MASK;
							txn->flags |= TX_CK_DOWN;
						}
					}
					srv = srv->next;
				}

				if (!srv && !(txn->flags & (TX_CK_DOWN|TX_CK_EXPIRED|TX_CK_OLD))) {
					 
					txn->flags &= ~TX_CK_MASK;
					if ((s->flags & (SF_IGNORE_PRST | SF_ASSIGNED)))
						txn->flags |= TX_CK_UNUSED;
					else
						txn->flags |= TX_CK_INVALID;
				}

				 
				if ((s->be->ck_opts & PR_CK_PFX) && (delim != val_end)) {
					int delta;  

					delta = buffer_replace2(req->buf, val_beg, delim + 1, NULL, 0);
					val_end  += delta;
					next     += delta;
					hdr_end  += delta;
					hdr_next += delta;
					cur_hdr->len += delta;
					http_msg_move_end(&txn->req, delta);

					del_from = NULL;
					preserve_hdr = 1;  
				}
				else if (del_from == NULL &&
					 (s->be->ck_opts & (PR_CK_INS | PR_CK_IND)) == (PR_CK_INS | PR_CK_IND)) {
					del_from = prev;
				}
			} else {
				 
				preserve_hdr = 1;

				if (del_from != NULL) {
					int delta = del_hdr_value(req->buf, &del_from, prev);
					if (att_beg >= del_from)
						att_beg += delta;
					if (att_end >= del_from)
						att_end += delta;
					val_beg  += delta;
					val_end  += delta;
					next     += delta;
					hdr_end  += delta;
					hdr_next += delta;
					cur_hdr->len += delta;
					http_msg_move_end(&txn->req, delta);
					prev     = del_from;
					del_from = NULL;
				}
			}

			 
			att_beg = next;
		}  

		 
		if (del_from) {
			int delta;
			if (preserve_hdr) {
				delta = del_hdr_value(req->buf, &del_from, hdr_end);
				hdr_end = del_from;
				cur_hdr->len += delta;
			} else {
				delta = buffer_replace2(req->buf, hdr_beg, hdr_next, NULL, 0);

				 
				txn->hdr_idx.v[old_idx].next = cur_hdr->next;
				txn->hdr_idx.used--;
				cur_hdr->len = 0;
				cur_idx = old_idx;
			}
			hdr_next += delta;
			http_msg_move_end(&txn->req, delta);
		}

		 
		old_idx = cur_idx;
	}
}
