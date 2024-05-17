static const char *parse_authority(struct parse_state *state)
{
	const char *tmp = state->ptr, *host = NULL;

	do {
		switch (*state->ptr) {
		case '@':
			 
			if (host) {
				TSRMLS_FETCH_FROM_CTX(state->ts);
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse userinfo; unexpected '@'");
				return NULL;
			}
			host = state->ptr + 1;
			if (tmp != state->ptr && SUCCESS != parse_userinfo(state, tmp)) {
				return NULL;
			}
			tmp = state->ptr + 1;
			break;

		case '/':
		case '?':
		case '#':
		case '\0':
			EOD:
			 
			if (tmp != state->ptr && SUCCESS != parse_hostinfo(state, tmp)) {
				return NULL;
			}
			return state->ptr;
		}
	} while (++state->ptr <= state->end);

	--state->ptr;
	goto EOD;
}