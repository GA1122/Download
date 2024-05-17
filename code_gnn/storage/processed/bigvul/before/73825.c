static ZEND_RESULT_CODE parse_hostinfo(struct parse_state *state, const char *ptr)
{
	size_t mb, len;
	const char *end = state->ptr, *tmp = ptr, *port = NULL, *label = NULL;
	TSRMLS_FETCH_FROM_CTX(state->ts);

#ifdef HAVE_INET_PTON
	if (*ptr == '[' && !(ptr = parse_ip6(state, ptr))) {
		return FAILURE;
	}
#endif

	if (ptr != end) do {
		switch (*ptr) {
		case ':':
			if (port) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse port; unexpected ':' at pos %u in '%s'",
						(unsigned) (ptr - tmp), tmp);
				return FAILURE;
			}
			port = ptr + 1;
			break;

		case '%':
			if (ptr[1] != '%' && (end - ptr <= 2 || !isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2)))) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse hostinfo; invalid percent encoding at pos %u in '%s'",
						(unsigned) (ptr - tmp), tmp);
				return FAILURE;
			}
			state->buffer[state->offset++] = *ptr++;
			state->buffer[state->offset++] = *ptr++;
			state->buffer[state->offset++] = *ptr;
			break;

		case '!': case '$': case '&': case '\'': case '(': case ')': case '*':
		case '+': case ',': case ';': case '=':  
		case '-': case '.': case '_': case '~':  
			if (port || !label) {
				 
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse %s; unexpected '%c' at pos %u in '%s'",
						port ? "port" : "host",
						(unsigned char) *ptr, (unsigned) (ptr - tmp), tmp);
				return FAILURE;
			}
			state->buffer[state->offset++] = *ptr;
			label = NULL;
			break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
		case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
		case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
		case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
		case 'v': case 'w': case 'x': case 'y': case 'z':
			if (port) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse port; unexpected char '%c' at pos %u in '%s'",
						(unsigned char) *ptr, (unsigned) (ptr - tmp), tmp);
				return FAILURE;
			}
			 
		case '0': case '1': case '2': case '3': case '4': case '5': case '6':
		case '7': case '8': case '9':
			 
			if (port) {
				state->url.port *= 10;
				state->url.port += *ptr - '0';
			} else {
				label = ptr;
				state->buffer[state->offset++] = *ptr;
			}
			break;

		default:
			if (ptr == end) {
				break;
			} else if (port) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse port; unexpected byte 0x%02x at pos %u in '%s'",
						(unsigned char) *ptr, (unsigned) (ptr - tmp), tmp);
				return FAILURE;
			} else if (!(mb = parse_mb(state, PARSE_HOSTINFO, ptr, end, tmp, 0))) {
				return FAILURE;
			}
			label = ptr;
			ptr += mb - 1;
		}
	} while (++ptr != end);

	if (!state->url.host) {
		len = (port ? port - tmp - 1 : end - tmp);
		state->url.host = &state->buffer[state->offset - len];
		state->buffer[state->offset++] = 0;
	}

	if (state->flags & PHP_HTTP_URL_PARSE_TOIDN) {
#if PHP_HTTP_HAVE_IDN2
		return parse_idn2(state, len);
#elif PHP_HTTP_HAVE_IDN
		return parse_idn(state, len);
#endif
#ifdef HAVE_UIDNA_IDNTOASCII
		return parse_uidn(state);
#endif
#if 0 && defined(PHP_WIN32)
		return parse_widn(state);
#endif
	}

	return SUCCESS;
}