smp_fetch_http_auth_grp(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	if (!args || args->type != ARGT_USR)
		return 0;

	CHECK_HTTP_MESSAGE_FIRST();

	if (!get_http_auth(smp->strm))
		return 0;

	 
	if (!check_user(args->data.usr, smp->strm->txn->auth.user,
	                smp->strm->txn->auth.pass))
		return 0;

	 
	smp->ctx.a[0] = args->data.usr;

	smp->data.type = SMP_T_STR;
	smp->flags = SMP_F_CONST;
	smp->data.u.str.str = smp->strm->txn->auth.user;
	smp->data.u.str.len = strlen(smp->strm->txn->auth.user);

	return 1;
}
