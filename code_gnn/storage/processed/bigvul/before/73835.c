static ZEND_RESULT_CODE parse_userinfo(struct parse_state *state, const char *ptr)
{
	size_t mb;
	const char *password = NULL, *end = state->ptr, *tmp = ptr;
	TSRMLS_FETCH_FROM_CTX(state->ts);

	state->url.user = &state->buffer[state->offset];

	do {
		switch (*ptr) {
		case ':':
			if (password) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse password; duplicate ':' at pos %u in '%s'",
						(unsigned) (ptr - tmp), tmp);
				return FAILURE;
			}
			password = ptr + 1;
			state->buffer[state->offset++] = 0;
			state->url.pass = &state->buffer[state->offset];
			break;

		case '%':
			if (ptr[1] != '%' && (end - ptr <= 2 || !isxdigit(*(ptr+1)) || !isxdigit(*(ptr+2)))) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse userinfo; invalid percent encoding at pos %u in '%s'",
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
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
		case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
		case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
		case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
		case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
		case 'v': case 'w': case 'x': case 'y': case 'z':
		case '0': case '1': case '2': case '3': case '4': case '5': case '6':
		case '7': case '8': case '9':
			 
			state->buffer[state->offset++] = *ptr;
			break;

		default:
			if (!(mb = parse_mb(state, PARSE_USERINFO, ptr, end, tmp, 0))) {
				return FAILURE;
			}
			ptr += mb - 1;
		}
	} while(++ptr != end);


	state->buffer[state->offset++] = 0;

	return SUCCESS;
}