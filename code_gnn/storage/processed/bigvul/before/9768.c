int apply_filter_to_resp_headers(struct session *s, struct channel *rtr, struct hdr_exp *exp)
{
	char *cur_ptr, *cur_end, *cur_next;
	int cur_idx, old_idx, last_hdr;
	struct http_txn *txn = &s->txn;
	struct hdr_idx_elem *cur_hdr;
	int delta;

	last_hdr = 0;

	cur_next = rtr->buf->p + hdr_idx_first_pos(&txn->hdr_idx);
	old_idx = 0;

	while (!last_hdr) {
		if (unlikely(txn->flags & TX_SVDENY))
			return 1;
		else if (unlikely(txn->flags & TX_SVALLOW) &&
			 (exp->action == ACT_ALLOW ||
			  exp->action == ACT_DENY))
			return 0;

		cur_idx = txn->hdr_idx.v[old_idx].next;
		if (!cur_idx)
			break;

		cur_hdr  = &txn->hdr_idx.v[cur_idx];
		cur_ptr  = cur_next;
		cur_end  = cur_ptr + cur_hdr->len;
		cur_next = cur_end + cur_hdr->cr + 1;

		 

		if (regex_exec_match2(exp->preg, cur_ptr, cur_end-cur_ptr, MAX_MATCH, pmatch)) {
			switch (exp->action) {
			case ACT_ALLOW:
				txn->flags |= TX_SVALLOW;
				last_hdr = 1;
				break;

			case ACT_DENY:
				txn->flags |= TX_SVDENY;
				last_hdr = 1;
				break;

			case ACT_REPLACE:
				trash.len = exp_replace(trash.str, trash.size, cur_ptr, exp->replace, pmatch);
				if (trash.len < 0)
					return -1;

				delta = buffer_replace2(rtr->buf, cur_ptr, cur_end, trash.str, trash.len);
				 

				cur_end += delta;
				cur_next += delta;
				cur_hdr->len += delta;
				http_msg_move_end(&txn->rsp, delta);
				break;

			case ACT_REMOVE:
				delta = buffer_replace2(rtr->buf, cur_ptr, cur_next, NULL, 0);
				cur_next += delta;

				http_msg_move_end(&txn->rsp, delta);
				txn->hdr_idx.v[old_idx].next = cur_hdr->next;
				txn->hdr_idx.used--;
				cur_hdr->len = 0;
				cur_end = NULL;  
				cur_idx = old_idx;
				break;

			}
		}

		 
		old_idx = cur_idx;
	}
	return 0;
}