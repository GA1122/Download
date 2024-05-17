int smp_fetch_cookie(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                 const struct arg *args, struct sample *smp, const char *kw)
{
	struct http_txn *txn = l7;
	struct hdr_idx *idx = &txn->hdr_idx;
	struct hdr_ctx *ctx = smp->ctx.a[2];
	const struct http_msg *msg;
	const char *hdr_name;
	int hdr_name_len;
	char *sol;
	int occ = 0;
	int found = 0;

	if (!args || args->type != ARGT_STR)
		return 0;

	if (!ctx) {
		 
		ctx = &static_hdr_ctx;
		ctx->idx = 0;
		smp->ctx.a[2] = ctx;
	}

	CHECK_HTTP_MESSAGE_FIRST();

	if ((opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ) {
		msg = &txn->req;
		hdr_name = "Cookie";
		hdr_name_len = 6;
	} else {
		msg = &txn->rsp;
		hdr_name = "Set-Cookie";
		hdr_name_len = 10;
	}

	if (!occ && !(opt & SMP_OPT_ITERATE))
		 
		occ = -1;

	 

	sol = msg->chn->buf->p;
	if (!(smp->flags & SMP_F_NOT_LAST)) {
		 
		smp->ctx.a[0] = NULL;
		ctx->idx = 0;
	}

	smp->flags |= SMP_F_VOL_HDR;

	while (1) {
		 
		if (!smp->ctx.a[0]) {
			if (!http_find_header2(hdr_name, hdr_name_len, sol, idx, ctx))
				goto out;

			if (ctx->vlen < args->data.str.len + 1)
				continue;

			smp->ctx.a[0] = ctx->line + ctx->val;
			smp->ctx.a[1] = smp->ctx.a[0] + ctx->vlen;
		}

		smp->type = SMP_T_STR;
		smp->flags |= SMP_F_CONST;
		smp->ctx.a[0] = extract_cookie_value(smp->ctx.a[0], smp->ctx.a[1],
						 args->data.str.str, args->data.str.len,
						 (opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ,
						 &smp->data.str.str,
						 &smp->data.str.len);
		if (smp->ctx.a[0]) {
			found = 1;
			if (occ >= 0) {
				 
				smp->flags |= SMP_F_NOT_LAST;
				return 1;
			}
		}
		 
	}
	 
 out:
	smp->flags &= ~SMP_F_NOT_LAST;
	return found;
}
