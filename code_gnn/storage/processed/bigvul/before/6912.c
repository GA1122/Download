smp_fetch_hdrs_bin(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct http_msg *msg;
	struct chunk *temp;
	struct hdr_idx *idx;
	const char *cur_ptr, *cur_next, *p;
	int old_idx, cur_idx;
	struct hdr_idx_elem *cur_hdr;
	const char *hn, *hv;
	int hnl, hvl;
	int ret;
	struct http_txn *txn;
	char *buf;
	char *end;

	CHECK_HTTP_MESSAGE_FIRST();

	temp = get_trash_chunk();
	buf = temp->str;
	end = temp->str + temp->size;

	txn = smp->strm->txn;
	idx = &txn->hdr_idx;
	msg = &txn->req;

	 
	old_idx = 0;
	cur_next = msg->chn->buf->p + hdr_idx_first_pos(idx);
	while (1) {
		cur_idx = idx->v[old_idx].next;
		if (!cur_idx)
			break;
		old_idx = cur_idx;

		cur_hdr  = &idx->v[cur_idx];
		cur_ptr  = cur_next;
		cur_next = cur_ptr + cur_hdr->len + cur_hdr->cr + 1;

		 

		 
		hn = cur_ptr;
		for (p = cur_ptr; p < cur_ptr + cur_hdr->len && *p != ':'; p++);
		if (p >= cur_ptr+cur_hdr->len)
			continue;
		hnl = p - hn;
		p++;
		while (p < cur_ptr + cur_hdr->len && (*p == ' ' || *p == '\t'))
			p++;
		if (p >= cur_ptr + cur_hdr->len)
			continue;
		hv = p;
		hvl = cur_ptr + cur_hdr->len-p;

		 
		ret = encode_varint(hnl, &buf, end);
		if (ret == -1)
			return 0;
		if (buf + hnl > end)
			return 0;
		memcpy(buf, hn, hnl);
		buf += hnl;

		 
		ret = encode_varint(hvl, &buf, end);
		if (ret == -1)
			return 0;
		if (buf + hvl > end)
			return 0;
		memcpy(buf, hv, hvl);
		buf += hvl;
	}

	 
	ret = encode_varint(0, &buf, end);
	if (ret == -1)
		return 0;
	ret = encode_varint(0, &buf, end);
	if (ret == -1)
		return 0;

	 
	smp->data.type = SMP_T_BIN;
	smp->data.u.str.str = temp->str;
	smp->data.u.str.len = buf - temp->str;
	smp->data.u.str.size = temp->size;

	return 1;
}
