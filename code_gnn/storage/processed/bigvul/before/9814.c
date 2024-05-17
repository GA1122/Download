int http_send_name_header(struct http_txn *txn, struct proxy* be, const char* srv_name) {

	struct hdr_ctx ctx;

	char *hdr_name = be->server_id_hdr_name;
	int hdr_name_len = be->server_id_hdr_len;
	struct channel *chn = txn->req.chn;
	char *hdr_val;
	unsigned int old_o, old_i;

	ctx.idx = 0;

	old_o = http_hdr_rewind(&txn->req);
	if (old_o) {
		 
		b_rew(chn->buf, old_o);
		txn->req.next += old_o;
		txn->req.sov  += old_o;
	}

	old_i = chn->buf->i;
	while (http_find_header2(hdr_name, hdr_name_len, txn->req.chn->buf->p, &txn->hdr_idx, &ctx)) {
		 
	        http_remove_header2(&txn->req, &txn->hdr_idx, &ctx);
	}

	 
	hdr_val = trash.str;
	memcpy(hdr_val, hdr_name, hdr_name_len);
	hdr_val += hdr_name_len;
	*hdr_val++ = ':';
	*hdr_val++ = ' ';
	hdr_val += strlcpy2(hdr_val, srv_name, trash.str + trash.size - hdr_val);
	http_header_add_tail2(&txn->req, &txn->hdr_idx, trash.str, hdr_val - trash.str);

	if (old_o) {
		 
		old_o += chn->buf->i - old_i;
		b_adv(chn->buf, old_o);
		txn->req.next -= old_o;
		txn->req.sov  -= old_o;
	}

	return 0;
}
