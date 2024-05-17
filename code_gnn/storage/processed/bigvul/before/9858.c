smp_fetch_proto_http(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                     const struct arg *args, struct sample *smp, const char *kw)
{
	 

	CHECK_HTTP_MESSAGE_FIRST_PERM();

	smp->type = SMP_T_BOOL;
	smp->data.uint = 1;
	return 1;
}
