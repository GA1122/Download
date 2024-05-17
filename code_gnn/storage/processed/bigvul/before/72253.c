input_userauth_success_unexpected(int type, u_int32_t seq, void *ctxt)
{
	Authctxt *authctxt = ctxt;

	if (authctxt == NULL)
		fatal("%s: no authentication context", __func__);

	fatal("Unexpected authentication success during %s.",
	    authctxt->method->name);
	return 0;
}
