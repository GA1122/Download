static int oidc_authorization_request_set_cookie(request_rec *r, oidc_cfg *c,
		const char *state, oidc_proto_state_t *proto_state) {
	 
	char *cookieValue = oidc_proto_state_to_cookie(r, c, proto_state);
	if (cookieValue == NULL)
		return HTTP_INTERNAL_SERVER_ERROR;

	 
	int number_of_cookies = oidc_clean_expired_state_cookies(r, c, NULL,
			oidc_cfg_delete_oldest_state_cookies(c));
	int max_number_of_cookies = oidc_cfg_max_number_of_state_cookies(c);
	if ((max_number_of_cookies > 0)
			&& (number_of_cookies >= max_number_of_cookies)) {

		oidc_warn(r,
				"the number of existing, valid state cookies (%d) has exceeded the limit (%d), no additional authorization request + state cookie can be generated, aborting the request",
				number_of_cookies, max_number_of_cookies);
		 

		 

		return HTTP_SERVICE_UNAVAILABLE;
	}

	 
	const char *cookieName = oidc_get_state_cookie_name(r, state);

	 
	oidc_util_set_cookie(r, cookieName, cookieValue, -1,
			c->cookie_same_site ? OIDC_COOKIE_EXT_SAME_SITE_LAX : NULL);

	return HTTP_OK;
}
