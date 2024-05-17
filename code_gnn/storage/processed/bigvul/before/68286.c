static int oidc_check_max_session_duration(request_rec *r, oidc_cfg *cfg,
		oidc_session_t *session) {
	const char *s_session_expires = NULL;
	apr_time_t session_expires;

	 
	oidc_session_get(r, session, OIDC_SESSION_EXPIRES_SESSION_KEY,
			&s_session_expires);

	 
	sscanf(s_session_expires, "%" APR_TIME_T_FMT, &session_expires);

	 
	if (apr_time_now() > session_expires) {
		oidc_warn(r, "maximum session duration exceeded for user: %s",
				session->remote_user);
		oidc_session_kill(r, session);
		return oidc_authenticate_user(r, cfg, NULL, oidc_get_current_url(r),
				NULL,
				NULL, NULL, NULL);
	}

	 
	oidc_log_session_expires(r, session_expires);

	return OK;
}
