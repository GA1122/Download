int select_compression_request_header(struct session *s, struct buffer *req)
{
	struct http_txn *txn = &s->txn;
	struct http_msg *msg = &txn->req;
	struct hdr_ctx ctx;
	struct comp_algo *comp_algo = NULL;
	struct comp_algo *comp_algo_back = NULL;

	 
	ctx.idx = 0;
	if (http_find_header2("User-Agent", 10, req->p, &txn->hdr_idx, &ctx) &&
	    ctx.vlen >= 9 &&
	    memcmp(ctx.line + ctx.val, "Mozilla/4", 9) == 0 &&
	    (ctx.vlen < 31 ||
	     memcmp(ctx.line + ctx.val + 25, "MSIE ", 5) != 0 ||
	     ctx.line[ctx.val + 30] < '6' ||
	     (ctx.line[ctx.val + 30] == '6' &&
	      (ctx.vlen < 54 || memcmp(ctx.line + 51, "SV1", 3) != 0)))) {
	    s->comp_algo = NULL;
	    return 0;
	}

	 
	if ((s->be->comp && (comp_algo_back = s->be->comp->algos)) || (s->fe->comp && (comp_algo_back = s->fe->comp->algos))) {
		int best_q = 0;

		ctx.idx = 0;
		while (http_find_header2("Accept-Encoding", 15, req->p, &txn->hdr_idx, &ctx)) {
			const char *qval;
			int q;
			int toklen;

			 
			toklen = 0;
			while (toklen < ctx.vlen && http_is_token[(unsigned char)*(ctx.line + ctx.val + toklen)])
				toklen++;

			qval = ctx.line + ctx.val + toklen;
			while (1) {
				while (qval < ctx.line + ctx.val + ctx.vlen && http_is_lws[(unsigned char)*qval])
					qval++;

				if (qval >= ctx.line + ctx.val + ctx.vlen || *qval != ';') {
					qval = NULL;
					break;
				}
				qval++;

				while (qval < ctx.line + ctx.val + ctx.vlen && http_is_lws[(unsigned char)*qval])
					qval++;

				if (qval >= ctx.line + ctx.val + ctx.vlen) {
					qval = NULL;
					break;
				}
				if (strncmp(qval, "q=", MIN(ctx.line + ctx.val + ctx.vlen - qval, 2)) == 0)
					break;

				while (qval < ctx.line + ctx.val + ctx.vlen && *qval != ';')
					qval++;
			}

			 
			q = qval ? parse_qvalue(qval + 2, NULL) : 1000;

			if (q <= best_q)
				continue;

			for (comp_algo = comp_algo_back; comp_algo; comp_algo = comp_algo->next) {
				if (*(ctx.line + ctx.val) == '*' ||
				    word_match(ctx.line + ctx.val, toklen, comp_algo->name, comp_algo->name_len)) {
					s->comp_algo = comp_algo;
					best_q = q;
					break;
				}
			}
		}
	}

	 
	if (s->comp_algo) {
		if ((s->be->comp && s->be->comp->offload) || (s->fe->comp && s->fe->comp->offload)) {
			http_remove_header2(msg, &txn->hdr_idx, &ctx);
			ctx.idx = 0;
			while (http_find_header2("Accept-Encoding", 15, req->p, &txn->hdr_idx, &ctx)) {
				http_remove_header2(msg, &txn->hdr_idx, &ctx);
			}
		}
		return 1;
	}

	 
	if ((s->be->comp && (comp_algo_back = s->be->comp->algos)) || (s->fe->comp && (comp_algo_back = s->fe->comp->algos))) {
		for (comp_algo = comp_algo_back; comp_algo; comp_algo = comp_algo->next) {
			if (comp_algo->add_data == identity_add_data) {
				s->comp_algo = comp_algo;
				return 1;
			}
		}
	}

	s->comp_algo = NULL;
	return 0;
}