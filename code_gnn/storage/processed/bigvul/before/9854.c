smp_fetch_http_auth_grp(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                        const struct arg *args, struct sample *smp, const char *kw)
{

	if (!args || args->type != ARGT_USR)
		return 0;

	CHECK_HTTP_MESSAGE_FIRST();

	if (!get_http_auth(l4))
		return 0;

	 
	if (!check_user(args->data.usr, l4->txn.auth.user, l4->txn.auth.pass))
		return 0;

	 
	smp->ctx.a[0] = args->data.usr;

	smp->type = SMP_T_STR;
	smp->flags = SMP_F_CONST;
	smp->data.str.str = l4->txn.auth.user;
	smp->data.str.len = strlen(l4->txn.auth.user);

	return 1;
}
