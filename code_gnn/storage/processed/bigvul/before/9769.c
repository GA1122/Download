int apply_filter_to_sts_line(struct session *s, struct channel *rtr, struct hdr_exp *exp)
{
	char *cur_ptr, *cur_end;
	int done;
	struct http_txn *txn = &s->txn;
	int delta;


	if (unlikely(txn->flags & TX_SVDENY))
		return 1;
	else if (unlikely(txn->flags & TX_SVALLOW) &&
		 (exp->action == ACT_ALLOW ||
		  exp->action == ACT_DENY))
		return 0;
	else if (exp->action == ACT_REMOVE)
		return 0;

	done = 0;

	cur_ptr = rtr->buf->p;
	cur_end = cur_ptr + txn->rsp.sl.st.l;

	 

	if (regex_exec_match2(exp->preg, cur_ptr, cur_end-cur_ptr, MAX_MATCH, pmatch)) {
		switch (exp->action) {
		case ACT_ALLOW:
			txn->flags |= TX_SVALLOW;
			done = 1;
			break;

		case ACT_DENY:
			txn->flags |= TX_SVDENY;
			done = 1;
			break;

		case ACT_REPLACE:
			trash.len = exp_replace(trash.str, trash.size, cur_ptr, exp->replace, pmatch);
			if (trash.len < 0)
				return -1;

			delta = buffer_replace2(rtr->buf, cur_ptr, cur_end, trash.str, trash.len);
			 

			http_msg_move_end(&txn->rsp, delta);
			cur_end += delta;
			cur_end = (char *)http_parse_stsline(&txn->rsp,
							     HTTP_MSG_RPVER,
							     cur_ptr, cur_end + 1,
							     NULL, NULL);
			if (unlikely(!cur_end))
				return -1;

			 
			txn->status = strl2ui(rtr->buf->p + txn->rsp.sl.st.c, txn->rsp.sl.st.c_l);
			hdr_idx_set_start(&txn->hdr_idx, txn->rsp.sl.st.l, *cur_end == '\r');
			 
			return 1;
		}
	}
	return done;
}
