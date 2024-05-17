apr_byte_t oidc_get_provider_from_session(request_rec *r, oidc_cfg *c,
		oidc_session_t *session, oidc_provider_t **provider) {

	oidc_debug(r, "enter");

	 
	const char *issuer = oidc_session_get_issuer(r, session);
	if (issuer == NULL) {
		oidc_error(r, "session corrupted: no issuer found in session");
		return FALSE;
	}

	 
	oidc_provider_t *p = oidc_get_provider_for_issuer(r, c, issuer, FALSE);
	if (p == NULL) {
		oidc_error(r, "session corrupted: no provider found for issuer: %s",
				issuer);
		return FALSE;
	}

	*provider = p;

	return TRUE;
}