static size_t parse_mb(struct parse_state *state, parse_mb_what_t what, const char *ptr, const char *end, const char *begin, zend_bool silent)
{
	unsigned wchar;
	size_t consumed = 0;

	if (state->flags & PHP_HTTP_URL_PARSE_MBUTF8) {
		consumed = parse_mb_utf8(&wchar, ptr, end);
	}
#ifdef PHP_HTTP_HAVE_WCHAR
	else if (state->flags & PHP_HTTP_URL_PARSE_MBLOC) {
		consumed = parse_mb_loc(&wchar, ptr, end);
	}
#endif

	while (consumed) {
		if (!(state->flags & PHP_HTTP_URL_PARSE_TOPCT) || what == PARSE_HOSTINFO || what == PARSE_SCHEME) {
			if (what == PARSE_HOSTINFO && (state->flags & PHP_HTTP_URL_PARSE_TOIDN)) {
				 
			} else if (state->flags & PHP_HTTP_URL_PARSE_MBUTF8) {
				if (!isualnum(wchar)) {
					break;
				}
#ifdef PHP_HTTP_HAVE_WCHAR
			} else if (state->flags & PHP_HTTP_URL_PARSE_MBLOC) {
				if (!iswalnum(wchar)) {
					break;
				}
#endif
			}
			PHP_HTTP_DUFF(consumed, state->buffer[state->offset++] = *ptr++);
		} else {
			int i = 0;

			PHP_HTTP_DUFF(consumed,
					state->buffer[state->offset++] = '%';
					state->buffer[state->offset++] = parse_xdigits[((unsigned char) ptr[i]) >> 4];
					state->buffer[state->offset++] = parse_xdigits[((unsigned char) ptr[i]) & 0xf];
					++i;
			);
		}

		return consumed;
	}

	if (!silent) {
		TSRMLS_FETCH_FROM_CTX(state->ts);
		if (consumed) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
					"Failed to parse %s; unexpected multibyte sequence 0x%x at pos %u in '%s'",
					parse_what[what], wchar, (unsigned) (ptr - begin), begin);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
					"Failed to parse %s; unexpected byte 0x%02x at pos %u in '%s'",
					parse_what[what], (unsigned char) *ptr, (unsigned) (ptr - begin), begin);
		}
	}

	return 0;
}