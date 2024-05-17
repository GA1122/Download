static int oidc_check_mixed_userid_oauth(request_rec *r, oidc_cfg *c) {

	 
	const char *access_token = NULL;
	if (oidc_oauth_get_bearer_token(r, &access_token) == TRUE)
		return oidc_oauth_check_userid(r, c, access_token);

	 
	return oidc_check_userid_openidc(r, c);
}
