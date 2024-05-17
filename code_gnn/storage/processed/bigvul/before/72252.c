input_userauth_success(int type, u_int32_t seq, void *ctxt)
{
	Authctxt *authctxt = ctxt;

	if (authctxt == NULL)
		fatal("input_userauth_success: no authentication context");
	free(authctxt->authlist);
	authctxt->authlist = NULL;
	if (authctxt->method != NULL && authctxt->method->cleanup != NULL)
		authctxt->method->cleanup(authctxt);
	free(authctxt->methoddata);
	authctxt->methoddata = NULL;
	authctxt->success = 1;			 
	return 0;
}
