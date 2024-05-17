static char *oidc_get_browser_state_hash(request_rec *r, const char *nonce) {

	oidc_debug(r, "enter");

	 
	const char *value = NULL;
	 
	apr_sha1_ctx_t sha1;

	 
	apr_sha1_init(&sha1);

	 
	value = (char *) apr_table_get(r->headers_in, "X_FORWARDED_FOR");
	 
	if (value != NULL)
		apr_sha1_update(&sha1, value, strlen(value));

	 
	value = (char *) apr_table_get(r->headers_in, "USER_AGENT");
	 
	if (value != NULL)
		apr_sha1_update(&sha1, value, strlen(value));

	 
	 

	 
	apr_sha1_update(&sha1, nonce, strlen(nonce));

	 
	unsigned char hash[OIDC_SHA1_LEN];
	apr_sha1_final(hash, &sha1);

	 
	char *result = NULL;
	oidc_base64url_encode(r, &result, (const char *) hash, OIDC_SHA1_LEN, TRUE);
	return result;
}
