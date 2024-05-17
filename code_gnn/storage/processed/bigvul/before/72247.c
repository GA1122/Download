input_userauth_failure(int type, u_int32_t seq, void *ctxt)
{
	Authctxt *authctxt = ctxt;
	char *authlist = NULL;
	int partial;

	if (authctxt == NULL)
		fatal("input_userauth_failure: no authentication context");

	authlist = packet_get_string(NULL);
	partial = packet_get_char();
	packet_check_eom();

	if (partial != 0) {
		verbose("Authenticated with partial success.");
		 
		pubkey_cleanup(authctxt);
		pubkey_prepare(authctxt);
	}
	debug("Authentications that can continue: %s", authlist);

	userauth(authctxt, authlist);
	return 0;
}
