int http_find_next_header(char *sol, struct hdr_idx *idx, struct hdr_ctx *ctx)
{
	char *eol, *sov;
	int cur_idx, old_idx;
	int len;

	cur_idx = ctx->idx;
	if (cur_idx) {
		 
		sol = ctx->line;
		eol = sol + idx->v[cur_idx].len;
		goto next_hdr;
	}

	 
	sol += hdr_idx_first_pos(idx);
	old_idx = 0;
	cur_idx = hdr_idx_first_idx(idx);
	while (cur_idx) {
		eol = sol + idx->v[cur_idx].len;

		len = 0;
		while (1) {
			if (len >= eol - sol)
				goto next_hdr;
			if (sol[len] == ':')
				break;
			len++;
		}

		ctx->del = len;
		sov = sol + len + 1;
		while (sov < eol && HTTP_IS_LWS(*sov))
			sov++;

		ctx->line = sol;
		ctx->prev = old_idx;
		ctx->idx  = cur_idx;
		ctx->val  = sov - sol;
		ctx->tws = 0;

		while (eol > sov && HTTP_IS_LWS(*(eol - 1))) {
			eol--;
			ctx->tws++;
		}
		ctx->vlen = eol - sov;
		return 1;

	next_hdr:
		sol = eol + idx->v[cur_idx].cr + 1;
		old_idx = cur_idx;
		cur_idx = idx->v[cur_idx].next;
	}
	return 0;
}