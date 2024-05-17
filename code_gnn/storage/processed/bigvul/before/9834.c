int select_compression_response_header(struct session *s, struct buffer *res)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &txn->rsp;
	struct hdr_ctx ctx;
	struct comp_type *comp_type;

	 
	if (s->comp_algo == NULL)
		goto fail;

	 
	if (!(msg->flags & HTTP_MSGF_VER_11) || !(txn->req.flags & HTTP_MSGF_VER_11))
		goto fail;

	 
	if (txn->status != 200)
		goto fail;

	 
	if (!(msg->flags & HTTP_MSGF_TE_CHNK) && msg->body_len == 0)
		goto fail;

	 
	ctx.idx = 0;
	if (http_find_header2("Content-Encoding", 16, res->p, &txn->hdr_idx, &ctx))
		goto fail;

	 
	ctx.idx = 0;
	while (http_find_header2("Cache-Control", 13, res->p, &txn->hdr_idx, &ctx)) {
		if (word_match(ctx.line + ctx.val, ctx.vlen, "no-transform", 12))
			goto fail;
	}

	comp_type = NULL;

	 
	ctx.idx = 0;
	if (http_find_header2("Content-Type", 12, res->p, &txn->hdr_idx, &ctx)) {
		if (ctx.vlen >= 9 && strncasecmp("multipart", ctx.line+ctx.val, 9) == 0)
			goto fail;

		if ((s->be->comp && (comp_type = s->be->comp->types)) ||
		    (s->fe->comp && (comp_type = s->fe->comp->types))) {
			for (; comp_type; comp_type = comp_type->next) {
				if (ctx.vlen >= comp_type->name_len &&
				    strncasecmp(ctx.line+ctx.val, comp_type->name, comp_type->name_len) == 0)
					 
					break;
			}
			 
			if (comp_type == NULL)
				goto fail;
		}
	}
	else {  
		if ((s->be->comp && s->be->comp->types) || (s->fe->comp && s->fe->comp->types))
			goto fail;  
	}

	 
	if (global.comp_rate_lim > 0)
		if (read_freq_ctr(&global.comp_bps_in) > global.comp_rate_lim)
			goto fail;

	 
	if (idle_pct < compress_min_idle)
		goto fail;

	 
	if (s->comp_algo->init(&s->comp_ctx, global.tune.comp_maxlevel) < 0)
		goto fail;

	s->flags |= SN_COMP_READY;

	 
	ctx.idx = 0;
	if ((msg->flags & HTTP_MSGF_CNT_LEN) && http_find_header2("Content-Length", 14, res->p, &txn->hdr_idx, &ctx))
		http_remove_header2(msg, &txn->hdr_idx, &ctx);

	 
	if (!(msg->flags & HTTP_MSGF_TE_CHNK))
		http_header_add_tail2(&txn->rsp, &txn->hdr_idx, "Transfer-Encoding: chunked", 26);

	 
	if (s->comp_algo->add_data != identity_add_data) {
		trash.len = 18;
		memcpy(trash.str, "Content-Encoding: ", trash.len);
		memcpy(trash.str + trash.len, s->comp_algo->name, s->comp_algo->name_len);
		trash.len += s->comp_algo->name_len;
		trash.str[trash.len] = '\0';
		http_header_add_tail2(&txn->rsp, &txn->hdr_idx, trash.str, trash.len);
	}
	return 1;

fail:
	s->comp_algo = NULL;
	return 0;
}
