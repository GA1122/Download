static int oidc_discovery(request_rec *r, oidc_cfg *cfg) {

	oidc_debug(r, "enter");

	 
	char *current_url = oidc_get_current_url(r);
	const char *method = oidc_original_request_method(r, cfg, FALSE);

	 
	char *csrf = NULL;
	if (oidc_proto_generate_nonce(r, &csrf, 8) == FALSE)
		return HTTP_INTERNAL_SERVER_ERROR;

	char *discover_url = oidc_cfg_dir_discover_url(r);
	 
	if (discover_url != NULL) {

		 
		char *url = apr_psprintf(r->pool, "%s%s%s=%s&%s=%s&%s=%s&%s=%s",
				discover_url, strchr(discover_url, '?') != NULL ? "&" : "?",
						OIDC_DISC_RT_PARAM, oidc_util_escape_string(r, current_url),
						OIDC_DISC_RM_PARAM, method,
						OIDC_DISC_CB_PARAM,
						oidc_util_escape_string(r, cfg->redirect_uri),
						OIDC_CSRF_NAME, oidc_util_escape_string(r, csrf));

		 
		oidc_debug(r, "redirecting to external discovery page: %s", url);

		 
		oidc_util_set_cookie(r, OIDC_CSRF_NAME, csrf, -1);

		 
		if (oidc_post_preserve_javascript(r, url, NULL, NULL) == TRUE)
			return DONE;

		 
		apr_table_add(r->headers_out, "Location", url);
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

		char *display =
				(strstr(issuer, "https://") == NULL) ?
						apr_pstrdup(r->pool, issuer) :
						apr_pstrdup(r->pool, issuer + strlen("https://"));

		 
		 
		s =
				apr_psprintf(r->pool,
						"%s<p><a href=\"%s?%s=%s&amp;%s=%s&amp;%s=%s&amp;%s=%s\">%s</a></p>\n",
						s, cfg->redirect_uri, OIDC_DISC_OP_PARAM,
						oidc_util_escape_string(r, issuer),
						OIDC_DISC_RT_PARAM,
						oidc_util_escape_string(r, current_url),
						OIDC_DISC_RM_PARAM, method,
						OIDC_CSRF_NAME, csrf, display);
	}

	 
	s = apr_psprintf(r->pool, "%s<form method=\"get\" action=\"%s\">\n", s,
			cfg->redirect_uri);
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
			OIDC_DISC_RT_PARAM, current_url);
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
			OIDC_DISC_RM_PARAM, method);
	s = apr_psprintf(r->pool,
			"%s<p><input type=\"hidden\" name=\"%s\" value=\"%s\"><p>\n", s,
			OIDC_CSRF_NAME, csrf);
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

	oidc_util_set_cookie(r, OIDC_CSRF_NAME, csrf, -1);

	char *javascript = NULL, *javascript_method = NULL;
	char *html_head =
			"<style type=\"text/css\">body {text-align: center}</style>";
	if (oidc_post_preserve_javascript(r, NULL, &javascript,
			&javascript_method) == TRUE)
		html_head = apr_psprintf(r->pool, "%s%s", html_head, javascript);

	 
	return oidc_util_html_send(r, "OpenID Connect Provider Discovery",
			html_head, javascript_method, s, DONE);
}
