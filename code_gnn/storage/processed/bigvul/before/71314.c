static int apply_credentials(git_buf *buf, http_subtransport *t)
{
	git_cred *cred = t->cred;
	git_http_auth_context *context;

	 
	if (!cred && t->connection_data.user && t->connection_data.pass) {
		if (!t->url_cred &&
			git_cred_userpass_plaintext_new(&t->url_cred,
				t->connection_data.user, t->connection_data.pass) < 0)
			return -1;

		cred = t->url_cred;
	}

	if (!cred)
		return 0;

	 
	if (auth_context_match(&context, t, credtype_match, &cred->credtype) < 0)
		return -1;

	return context->next_token(buf, context, cred);
}
