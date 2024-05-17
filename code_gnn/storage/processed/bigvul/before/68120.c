authz_status oidc_authz_checker(request_rec *r, const char *require_args, const void *parsed_require_args) {

	 
	if (r->user != NULL && strlen(r->user) == 0) {
		r->user = NULL;
		if (oidc_dir_cfg_unauth_action(r) == OIDC_UNAUTH_PASS) return AUTHZ_GRANTED;
	}

	 
	json_t *claims = NULL, *id_token = NULL;
	oidc_authz_get_claims_and_idtoken(r, &claims, &id_token);

	 
	if (claims)
		oidc_util_json_merge(id_token, claims);

	 
	authz_status rc = oidc_authz_worker24(r, claims ? claims : id_token, require_args);

	 
	if (claims) json_decref(claims);
	if (id_token) json_decref(id_token);

	if ((rc == AUTHZ_DENIED) && ap_auth_type(r)
			&& (apr_strnatcasecmp((const char *) ap_auth_type(r), "oauth20")
					== 0))
		oidc_oauth_return_www_authenticate(r, "insufficient_scope", "Different scope(s) or other claims required");

	return rc;
}
