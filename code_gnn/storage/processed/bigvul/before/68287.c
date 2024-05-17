static int oidc_check_userid_openidc(request_rec *r, oidc_cfg *c) {

	if (c->redirect_uri == NULL) {
		oidc_error(r,
				"configuration error: the authentication type is set to \"openid-connect\" but OIDCRedirectURI has not been set");
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	 
	if (ap_is_initial_req(r)) {

		int rc = OK;

		 
		oidc_session_t *session = NULL;
		oidc_session_load(r, &session);

		 
		if (oidc_util_request_matches_url(r, c->redirect_uri)) {

			 
			rc = oidc_handle_redirect_uri_request(r, c, session);

			 
			oidc_session_free(r, session);

			return rc;

		 
		} else if (session->remote_user != NULL) {

			 
			r->user = (char *) session->remote_user;

			 
			rc = oidc_handle_existing_session(r, c, session);

			 
			oidc_session_free(r, session);

			 
			oidc_strip_cookies(r);

			return rc;
		}

		 
		oidc_session_free(r, session);

		 
	} else {

		 
		if (r->main != NULL)
			r->user = r->main->user;
		else if (r->prev != NULL)
			r->user = r->prev->user;

		if (r->user != NULL) {

			 
			oidc_debug(r,
					"recycling user '%s' from initial request for sub-request",
					r->user);

			 
			const char *s_id_token = oidc_request_state_get(r,
					OIDC_IDTOKEN_CLAIMS_SESSION_KEY);
			if (s_id_token == NULL) {

				oidc_session_t *session = NULL;
				oidc_session_load(r, &session);

				oidc_copy_tokens_to_request_state(r, session, NULL, NULL);

				 
				oidc_session_free(r, session);
			}

			 
			oidc_strip_cookies(r);

			return OK;
		}
		 
	}

	 
	switch (oidc_dir_cfg_unauth_action(r)) {
		case OIDC_UNAUTH_RETURN410:
			return HTTP_GONE;
		case OIDC_UNAUTH_RETURN401:
			return HTTP_UNAUTHORIZED;
		case OIDC_UNAUTH_PASS:
			r->user = "";
			return OK;
		case OIDC_UNAUTH_AUTHENTICATE:
			 
			if (apr_table_get(r->headers_in, "X-Requested-With") != NULL)
				return HTTP_UNAUTHORIZED;
			break;
	}

	 
	return oidc_authenticate_user(r, c, NULL, oidc_get_current_url(r), NULL,
			NULL, NULL, NULL);
}
