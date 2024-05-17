static char *oidc_get_browser_state_hash(request_rec *r, const char *nonce) {

	oidc_debug(r, "enter");

	 
	const char *value = NULL;
	 
	apr_sha1_ctx_t sha1;

	 
	apr_sha1_init(&sha1);

	 
	value = oidc_util_hdr_in_x_forwarded_for_get(r);
	 
	if (value != NULL)
		apr_sha1_update(&sha1, value, strlen(value));

	 
	value = oidc_util_hdr_in_user_agent_get(r);
	 
	if (value != NULL)
		apr_sha1_update(&sha1, value, strlen(value));

	 
	 

	 
	apr_sha1_update(&sha1, nonce, strlen(nonce));

	 
	value = oidc_util_get_provided_token_binding_id(r);
	if (value != NULL) {
		oidc_debug(r,
				"Provided Token Binding ID environment variable found; adding its value to the state");
		apr_sha1_update(&sha1, value, strlen(value));
	}

	 
	unsigned char hash[OIDC_SHA1_LEN];
	apr_sha1_final(hash, &sha1);

	 
	char *result = NULL;
	oidc_base64url_encode(r, &result, (const char *) hash, OIDC_SHA1_LEN, TRUE);
	return result;
}
