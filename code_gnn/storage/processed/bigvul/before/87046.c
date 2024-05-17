int oidc_auth_checker(request_rec *r) {

	 
	if (r->user != NULL && strlen(r->user) == 0) {
		r->user = NULL;
		if (oidc_dir_cfg_unauth_action(r) == OIDC_UNAUTH_PASS)
			return OK;
	}

	 
	json_t *claims = NULL, *id_token = NULL;
	oidc_authz_get_claims_and_idtoken(r, &claims, &id_token);

	 
	const apr_array_header_t * const reqs_arr = ap_requires(r);

	 
	const require_line * const reqs =
			reqs_arr ? (require_line *) reqs_arr->elts : NULL;
	if (!reqs_arr) {
		oidc_debug(r,
				"no require statements found, so declining to perform authorization.");
		return DECLINED;
	}

	 
	if (claims)
		oidc_util_json_merge(r, id_token, claims);

	 
	int rc = oidc_authz_worker22(r, claims ? claims : id_token, reqs,
			reqs_arr->nelts);

	 
	if (claims)
		json_decref(claims);
	if (id_token)
		json_decref(id_token);

	if ((rc == HTTP_UNAUTHORIZED) && ap_auth_type(r))
		rc = oidc_handle_unauthorized_user22(r);

	return rc;
}
