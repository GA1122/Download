input_gssapi_response(int type, u_int32_t plen, void *ctxt)
{
	Authctxt *authctxt = ctxt;
	Gssctxt *gssctxt;
	int oidlen;
	char *oidv;

	if (authctxt == NULL)
		fatal("input_gssapi_response: no authentication context");
	gssctxt = authctxt->methoddata;

	 
	oidv = packet_get_string(&oidlen);

	if (oidlen <= 2 ||
	    oidv[0] != SSH_GSS_OIDTYPE ||
	    oidv[1] != oidlen - 2) {
		free(oidv);
		debug("Badly encoded mechanism OID received");
		userauth(authctxt, NULL);
		return 0;
	}

	if (!ssh_gssapi_check_oid(gssctxt, oidv + 2, oidlen - 2))
		fatal("Server returned different OID than expected");

	packet_check_eom();

	free(oidv);

	if (GSS_ERROR(process_gssapi_token(ctxt, GSS_C_NO_BUFFER))) {
		 
		debug("Trying to start again");
		userauth(authctxt, NULL);
		return 0;
	}
	return 0;
}
