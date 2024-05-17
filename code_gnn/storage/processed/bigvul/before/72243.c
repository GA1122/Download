input_gssapi_token(int type, u_int32_t plen, void *ctxt)
{
	Authctxt *authctxt = ctxt;
	gss_buffer_desc recv_tok;
	OM_uint32 status;
	u_int slen;

	if (authctxt == NULL)
		fatal("input_gssapi_response: no authentication context");

	recv_tok.value = packet_get_string(&slen);
	recv_tok.length = slen;	 

	packet_check_eom();

	status = process_gssapi_token(ctxt, &recv_tok);

	free(recv_tok.value);

	if (GSS_ERROR(status)) {
		 
		userauth(authctxt, NULL);
		return 0;
	}
	return 0;
}
