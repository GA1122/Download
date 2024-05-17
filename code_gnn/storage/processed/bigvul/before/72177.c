mm_answer_gss_userok(int sock, Buffer *m)
{
	int authenticated;

	if (!options.gss_authentication)
		fatal("%s: GSSAPI authentication not enabled", __func__);

	authenticated = authctxt->valid && ssh_gssapi_userok(authctxt->user);

	buffer_clear(m);
	buffer_put_int(m, authenticated);

	debug3("%s: sending result %d", __func__, authenticated);
	mm_request_send(sock, MONITOR_ANS_GSSUSEROK, m);

	auth_method = "gssapi-with-mic";

	 
	return (authenticated);
}