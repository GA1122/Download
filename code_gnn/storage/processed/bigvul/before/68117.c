static apr_byte_t oidc_authorization_request_set_cookie(request_rec *r,
		oidc_cfg *c, const char *state, json_t *proto_state) {
	 

	 
	char *cookieValue = NULL;

	if (oidc_util_jwt_create(r, c->crypto_passphrase, proto_state,
			&cookieValue) == FALSE)
		return FALSE;

	 
	oidc_clean_expired_state_cookies(r, c);

	 
	const char *cookieName = oidc_get_state_cookie_name(r, state);

	 
	oidc_util_set_cookie(r, cookieName, cookieValue, -1);


	return TRUE;
}
