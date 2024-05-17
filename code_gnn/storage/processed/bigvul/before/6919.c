smp_fetch_proto_http(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	 

	CHECK_HTTP_MESSAGE_FIRST_PERM();

	smp->data.type = SMP_T_BOOL;
	smp->data.u.sint = 1;
	return 1;
}
