int oidc_check_user_id(request_rec *r) {

	oidc_cfg *c = ap_get_module_config(r->server->module_config,
			&auth_openidc_module);

	 
	oidc_debug(r, "incoming request: \"%s?%s\", ap_is_initial_req(r)=%d",
			r->parsed_uri.path, r->args, ap_is_initial_req(r));

	 
	if (ap_auth_type(r) == NULL)
		return DECLINED;

	 
	if (apr_strnatcasecmp((const char *) ap_auth_type(r), "openid-connect")
			== 0)
		return oidc_check_userid_openidc(r, c);

	 
	if (apr_strnatcasecmp((const char *) ap_auth_type(r), "oauth20") == 0)
		return oidc_oauth_check_userid(r, c);

	 
	return DECLINED;
}