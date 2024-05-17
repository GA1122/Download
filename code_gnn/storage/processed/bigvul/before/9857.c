smp_fetch_path(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
               const struct arg *args, struct sample *smp, const char *kw)
{
	struct http_txn *txn = l7;
	char *ptr, *end;

	CHECK_HTTP_MESSAGE_FIRST();

	end = txn->req.chn->buf->p + txn->req.sl.rq.u + txn->req.sl.rq.u_l;
	ptr = http_get_path(txn);
	if (!ptr)
		return 0;

	 
	smp->type = SMP_T_STR;
	smp->data.str.str = ptr;

	while (ptr < end && *ptr != '?')
		ptr++;

	smp->data.str.len = ptr - smp->data.str.str;
	smp->flags = SMP_F_VOL_1ST | SMP_F_CONST;
	return 1;
}