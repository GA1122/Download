static int oidc_discovery(request_rec *r, oidc_cfg *cfg) {

	oidc_debug(r, "enter");

	 
	char *current_url = oidc_get_current_url(r);
	const char *method = oidc_original_request_method(r, cfg, FALSE);

	 
	char *csrf = NULL;
	if (oidc_proto_generate_nonce(r, &csrf, 8) == FALSE)
		return HTTP_INTERNAL_SERVER_ERROR;

	char *path_scopes = oidc_dir_cfg_path_scope(r);
	char *path_auth_request_params = oidc_dir_cfg_path_auth_request_params(r);

	char *discover_url = oidc_cfg_dir_discover_url(r);
	 
	if (discover_url != NULL) {

		 
		char *url = apr_psprintf(r->pool, "%s%s%s=%s&%s=%s&%s=%s&%s=%s",
				discover_url,
				strchr(discover_url, OIDC_CHAR_QUERY) != NULL ?
						OIDC_STR_AMP :
						OIDC_STR_QUERY,
						OIDC_DISC_RT_PARAM, oidc_util_escape_string(r, current_url),
						OIDC_DISC_RM_PARAM, method,
						OIDC_DISC_CB_PARAM,
						oidc_util_escape_string(r, oidc_get_redirect_uri(r, cfg)),
						OIDC_CSRF_NAME, oidc_util_escape_string(r, csrf));

		if (path_scopes != NULL)
			url = apr_psprintf(r->pool, "%s&%s=%s", url, OIDC_DISC_SC_PARAM,
					oidc_util_escape_string(r, path_scopes));
		if (path_auth_request_params != NULL)
			url = apr_psprintf(r->pool, "%s&%s=%s", url, OIDC_DISC_AR_PARAM,
					oidc_util_escape_string(r, path_auth_request_params));

		 
		oidc_debug(r, "redirecting to external discovery page: %s", url);

		 
		oidc_util_set_cookie(r, OIDC_CSRF_NAME, csrf, -1,
				cfg->cookie_same_site ?
						OIDC_COOKIE_EXT_SAME_SITE_STRICT :
						NULL);

		 
		if (oidc_post_preserve_javascript(r, url, NULL, NULL) == TRUE)
			return DONE;

		 
		oidc_util_hdr_out_location_set(r, url);

		return HTTP_MOVED_TEMPORARILY;
	}

	 
	apr_array_header_t *arr = NULL;
	if (oidc_metadata_list(r, cfg, &arr) == FALSE)
		return oidc_util_html_send_error(r, cfg->error_template,
				"Configuration Error",
				"No configured providers found, contact your administrator",
				HTTP_UNAUTHORIZED);

	 
	const char *s = "			<h3>Select your OpenID Connect Identity Provider</h3>\n";

	 
	int i;
	for (i = 0; i < arr->nelts; i++) {

		const char *issuer = ((const char**) arr->elts)[i];

		char *href = apr_psprintf(r->pool,
				"%s?%s=%s&amp;%s=%s&amp;%s=%s&amp;%s=%s",
				oidc_get_redirect_uri(r, cfg), OIDC_DISC_OP_PARAM,
				oidc_util_escape_string(r, issuer),
				OIDC_DISC_RT_PARAM, oidc_util_escape_string(r, current_url),
				OIDC_DISC_RM_PARAM, method,
				OIDC_CSRF_NAME, csrf);

		if (path_scopes != NULL)
			href = apr_psprintf(r->pool, "%s&amp;%s=%s", href,
					OIDC_DISC_SC_PARAM, oidc_util_escape_string(r, path_scopes));
		if (path_auth_request_params != NULL)
			href = apr_psprintf(r->pool, "%s&amp;%s=%s", href,
					OIDC_DISC_AR_PARAM,
					oidc_util_escape_string(r, path_auth_request_params));

		char *display =
				(strstr(issuer, "https://") == NULL) ?
						apr_pstrdup(r->pool, issuer) :
						apr_pstrdup(r->pool, issuer + strlen("https://"));

		 
		 
		s = apr_psprintf(r->pool, "%s<p><a href=\"%s\">%s</a></p>\n", s, href,
				display);
	}

	 
	s = apr_psprintf(r->pool, "%s<form method=\"get\" action=\"%s\">\n", s,
			oidc_get_redirect_uri(r, cfg));
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
			OIDC_DISC_RT_PARAM, current_url);
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
			OIDC_DISC_RM_PARAM, method);
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
			OIDC_CSRF_NAME, csrf);

	if (path_scopes != NULL)
		s = apr_psprintf(r->pool,
				"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
				OIDC_DISC_SC_PARAM, path_scopes);
	if (path_auth_request_params != NULL)
		s = apr_psprintf(r->pool,
				"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
				OIDC_DISC_AR_PARAM, path_auth_request_params);

	s =
			apr_psprintf(r->pool,
					"%s<p>Or enter your account name (eg. &quot;mike@seed.gluu.org&quot;, or an IDP identifier (eg. &quot;mitreid.org&quot;):</p>\n",
					s);
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"text\" name=\"%s\" value=\"%s\"></p>\n", s,
			OIDC_DISC_OP_PARAM, "");
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"submit\" value=\"Submit\"></p>\n", s);
	s = apr_psprintf(r->pool, "%s</form>\n", s);

	oidc_util_set_cookie(r, OIDC_CSRF_NAME, csrf, -1,
			cfg->cookie_same_site ? OIDC_COOKIE_EXT_SAME_SITE_STRICT : NULL);

	char *javascript = NULL, *javascript_method = NULL;
	char *html_head =
			"<style type=\"text/css\">body {text-align: center}</style>";
	if (oidc_post_preserve_javascript(r, NULL, &javascript,
			&javascript_method) == TRUE)
		html_head = apr_psprintf(r->pool, "%s%s", html_head, javascript);

	 
	return oidc_util_html_send(r, "OpenID Connect Provider Discovery",
			html_head, javascript_method, s, DONE);
}
