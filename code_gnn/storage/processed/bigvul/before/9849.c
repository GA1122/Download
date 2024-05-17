smp_fetch_hdr(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
              const struct arg *args, struct sample *smp, const char *kw)
{
	struct http_txn *txn = l7;
	struct hdr_idx *idx = &txn->hdr_idx;
	struct hdr_ctx *ctx = smp->ctx.a[0];
	const struct http_msg *msg = ((opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ) ? &txn->req : &txn->rsp;
	int occ = 0;
	const char *name_str = NULL;
	int name_len = 0;

	if (!ctx) {
		 
		ctx = &static_hdr_ctx;
		ctx->idx = 0;
		smp->ctx.a[0] = ctx;
	}

	if (args) {
		if (args[0].type != ARGT_STR)
			return 0;
		name_str = args[0].data.str.str;
		name_len = args[0].data.str.len;

		if (args[1].type == ARGT_UINT || args[1].type == ARGT_SINT)
			occ = args[1].data.uint;
	}

	CHECK_HTTP_MESSAGE_FIRST();

	if (ctx && !(smp->flags & SMP_F_NOT_LAST))
		 
		ctx->idx = 0;

	if (!occ && !(opt & SMP_OPT_ITERATE))
		 
		occ = -1;

	if (!occ)
		 
		smp->flags |= SMP_F_NOT_LAST;

	smp->type = SMP_T_STR;
	smp->flags |= SMP_F_VOL_HDR | SMP_F_CONST;
	if (http_get_hdr(msg, name_str, name_len, idx, occ, ctx, &smp->data.str.str, &smp->data.str.len))
		return 1;

	smp->flags &= ~SMP_F_NOT_LAST;
	return 0;
}