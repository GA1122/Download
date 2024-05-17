void manage_server_side_cookies(struct session *s, struct channel *res)
{
	struct http_txn *txn = &s->txn;
	struct server *srv;
	int is_cookie2;
	int cur_idx, old_idx, delta;
	char *hdr_beg, *hdr_end, *hdr_next;
	char *prev, *att_beg, *att_end, *equal, *val_beg, *val_end, *next;

	 
	old_idx = 0;
	hdr_next = res->buf->p + hdr_idx_first_pos(&txn->hdr_idx);

	while ((cur_idx = txn->hdr_idx.v[old_idx].next)) {
		struct hdr_idx_elem *cur_hdr;
		int val;

		cur_hdr  = &txn->hdr_idx.v[cur_idx];
		hdr_beg  = hdr_next;
		hdr_end  = hdr_beg + cur_hdr->len;
		hdr_next = hdr_end + cur_hdr->cr + 1;

		 

		is_cookie2 = 0;
		prev = hdr_beg + 10;
		val = http_header_match2(hdr_beg, hdr_end, "Set-Cookie", 10);
		if (!val) {
			val = http_header_match2(hdr_beg, hdr_end, "Set-Cookie2", 11);
			if (!val) {
				old_idx = cur_idx;
				continue;
			}
			is_cookie2 = 1;
			prev = hdr_beg + 11;
		}

		 
		txn->flags |= TX_SCK_PRESENT;

		 
		if (s->be->cookie_name == NULL &&
		    s->be->appsession_name == NULL &&
		    s->fe->capture_name == NULL)
			return;

		 

		for (; prev < hdr_end; prev = next) {
			 

			 
			att_beg = prev + 1;
			while (att_beg < hdr_end && http_is_spht[(unsigned char)*att_beg])
				att_beg++;

			 
			equal = att_end = att_beg;

			while (equal < hdr_end) {
				if (*equal == '=' || *equal == ';' || (is_cookie2 && *equal == ','))
					break;
				if (http_is_spht[(unsigned char)*equal++])
					continue;
				att_end = equal;
			}

			 

			 
			if (equal < hdr_end && *equal == '=') {
				 
				val_beg = equal + 1;
				while (val_beg < hdr_end && http_is_spht[(unsigned char)*val_beg])
					val_beg++;

				 
				next = find_cookie_value_end(val_beg, hdr_end);

				 
				val_end = next;
				while (val_end > val_beg && http_is_spht[(unsigned char)*(val_end - 1)])
					val_end--;
			} else {
				 
				val_beg = val_end = next = equal;
			}

			if (next < hdr_end) {
				 
				if (is_cookie2)
					next = find_hdr_value_end(next, hdr_end);
				else
					next = hdr_end;
			}

			 

			 
			if (equal == val_end)
				continue;

			 
			if (unlikely(att_end != equal || val_beg > equal + 1)) {
				int stripped_before = 0;
				int stripped_after = 0;

				if (att_end != equal) {
					stripped_before = buffer_replace2(res->buf, att_end, equal, NULL, 0);
					equal   += stripped_before;
					val_beg += stripped_before;
				}

				if (val_beg > equal + 1) {
					stripped_after = buffer_replace2(res->buf, equal + 1, val_beg, NULL, 0);
					val_beg += stripped_after;
					stripped_before += stripped_after;
				}

				val_end      += stripped_before;
				next         += stripped_before;
				hdr_end      += stripped_before;
				hdr_next     += stripped_before;
				cur_hdr->len += stripped_before;
				http_msg_move_end(&txn->rsp, stripped_before);
			}

			 
			if (s->fe->capture_name != NULL &&
			    txn->srv_cookie == NULL &&
			    (val_end - att_beg >= s->fe->capture_namelen) &&
			    memcmp(att_beg, s->fe->capture_name, s->fe->capture_namelen) == 0) {
				int log_len = val_end - att_beg;
				if ((txn->srv_cookie = pool_alloc2(pool2_capture)) == NULL) {
					Alert("HTTP logging : out of memory.\n");
				}
				else {
					if (log_len > s->fe->capture_len)
						log_len = s->fe->capture_len;
					memcpy(txn->srv_cookie, att_beg, log_len);
					txn->srv_cookie[log_len] = 0;
				}
			}

			srv = objt_server(s->target);
			 
			if (!(s->flags & SN_IGNORE_PRST) &&
			    (att_end - att_beg == s->be->cookie_len) && (s->be->cookie_name != NULL) &&
			    (memcmp(att_beg, s->be->cookie_name, att_end - att_beg) == 0)) {
				 
				txn->flags &= ~TX_SCK_MASK;
				txn->flags |= TX_SCK_FOUND;
			
				 
				if (s->be->ck_opts & PR_CK_PSV) {
					 
				}
				else if ((srv && (s->be->ck_opts & PR_CK_INS)) ||
				    ((s->flags & SN_DIRECT) && (s->be->ck_opts & PR_CK_IND))) {
					 
					if (*prev == ':' && next == hdr_end) {
						 
						delta = buffer_replace2(res->buf, hdr_beg, hdr_next, NULL, 0);
						txn->hdr_idx.v[old_idx].next = cur_hdr->next;
						txn->hdr_idx.used--;
						cur_hdr->len = 0;
						cur_idx = old_idx;
						hdr_next += delta;
						http_msg_move_end(&txn->rsp, delta);
						 
					} else {
						 
						int delta = del_hdr_value(res->buf, &prev, next);
						next      = prev;
						hdr_end  += delta;
						hdr_next += delta;
						cur_hdr->len += delta;
						http_msg_move_end(&txn->rsp, delta);
					}
					txn->flags &= ~TX_SCK_MASK;
					txn->flags |= TX_SCK_DELETED;
					 
				}
				else if (srv && srv->cookie && (s->be->ck_opts & PR_CK_RW)) {
					 
					delta = buffer_replace2(res->buf, val_beg, val_end, srv->cookie, srv->cklen);
					next     += delta;
					hdr_end  += delta;
					hdr_next += delta;
					cur_hdr->len += delta;
					http_msg_move_end(&txn->rsp, delta);

					txn->flags &= ~TX_SCK_MASK;
					txn->flags |= TX_SCK_REPLACED;
				}
				else if (srv && srv->cookie && (s->be->ck_opts & PR_CK_PFX)) {
					 
					delta = buffer_replace2(res->buf, val_beg, val_beg, srv->cookie, srv->cklen + 1);
					next     += delta;
					hdr_end  += delta;
					hdr_next += delta;
					cur_hdr->len += delta;
					http_msg_move_end(&txn->rsp, delta);

					val_beg[srv->cklen] = COOKIE_DELIM;
					txn->flags &= ~TX_SCK_MASK;
					txn->flags |= TX_SCK_REPLACED;
				}
			}
			 
			else if (!(s->flags & SN_IGNORE_PRST) && (s->be->appsession_name != NULL)) {
				int cmp_len, value_len;
				char *value_begin;

				if (s->be->options2 & PR_O2_AS_PFX) {
					cmp_len = MIN(val_end - att_beg, s->be->appsession_name_len);
					value_begin = att_beg + s->be->appsession_name_len;
					value_len = MIN(s->be->appsession_len, val_end - att_beg - s->be->appsession_name_len);
				} else {
					cmp_len = att_end - att_beg;
					value_begin = val_beg;
					value_len = MIN(s->be->appsession_len, val_end - val_beg);
				}

				if ((cmp_len == s->be->appsession_name_len) &&
				    (memcmp(att_beg, s->be->appsession_name, s->be->appsession_name_len) == 0)) {
					 
					pool_free2(apools.sessid, txn->sessid);

					 
					if ((txn->sessid = pool_alloc2(apools.sessid)) == NULL) {
						Alert("Not enough Memory process_srv():asession->sessid:malloc().\n");
						send_log(s->be, LOG_ALERT, "Not enough Memory process_srv():asession->sessid:malloc().\n");
						return;
					}
					memcpy(txn->sessid, value_begin, value_len);
					txn->sessid[value_len] = 0;
				}
			}
			 
		}
		 
		old_idx = cur_idx;
	}

	if (txn->sessid != NULL) {
		appsess *asession = NULL;
		 
		asession = appsession_hash_lookup(&(s->be->htbl_proxy), txn->sessid);
		if (asession == NULL) {
			size_t server_id_len;
			if ((asession = pool_alloc2(pool2_appsess)) == NULL) {
				Alert("Not enough Memory process_srv():asession:calloc().\n");
				send_log(s->be, LOG_ALERT, "Not enough Memory process_srv():asession:calloc().\n");
				return;
			}
			asession->serverid = NULL;  

			if ((asession->sessid = pool_alloc2(apools.sessid)) == NULL) {
				Alert("Not enough Memory process_srv():asession->sessid:malloc().\n");
				send_log(s->be, LOG_ALERT, "Not enough Memory process_srv():asession->sessid:malloc().\n");
				s->be->htbl_proxy.destroy(asession);
				return;
			}
			memcpy(asession->sessid, txn->sessid, s->be->appsession_len);
			asession->sessid[s->be->appsession_len] = 0;

			server_id_len = strlen(objt_server(s->target)->id) + 1;
			if ((asession->serverid = pool_alloc2(apools.serverid)) == NULL) {
				Alert("Not enough Memory process_srv():asession->serverid:malloc().\n");
				send_log(s->be, LOG_ALERT, "Not enough Memory process_srv():asession->sessid:malloc().\n");
				s->be->htbl_proxy.destroy(asession);
				return;
			}
			asession->serverid[0] = '\0';
			memcpy(asession->serverid, objt_server(s->target)->id, server_id_len);

			asession->request_count = 0;
			appsession_hash_insert(&(s->be->htbl_proxy), asession);
		}

		asession->expire = tick_add_ifset(now_ms, s->be->timeout.appsession);
		asession->request_count++;
	}
}
