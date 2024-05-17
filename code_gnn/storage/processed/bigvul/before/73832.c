static const char *parse_path(struct parse_state *state)
{
	size_t mb;
	const char *tmp;
	TSRMLS_FETCH_FROM_CTX(state->ts);

	 
	if (!*state->ptr) {
		return state->ptr;
	}
	tmp = state->ptr;
	state->url.path = &state->buffer[state->offset];

	do {
		switch (*state->ptr) {
		case '#':
		case '?':
			goto done;

		case '%':
			if (state->ptr[1] != '%' && (state->end - state->ptr <= 2 || !isxdigit(*(state->ptr+1)) || !isxdigit(*(state->ptr+2)))) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Failed to parse path; invalid percent encoding at pos %u in '%s'",
						(unsigned) (state->ptr - tmp), tmp);
				return NULL;
			}
			state->buffer[state->offset++] = *state->ptr++;
			state->buffer[state->offset++] = *state->ptr++;
			state->buffer[state->offset++] = *state->ptr;
			break;

		case '/':  
		case '!': case '$': case '&': case '\'': case '(': case ')': case '*':
		case '+': case ',': case ';': case '=':  
		case '-': case '.': case '_': case '~':  
		case ':': case '@':  
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
			 
			state->buffer[state->offset++] = *state->ptr;
			break;

		default:
			if (!(mb = parse_mb(state, PARSE_PATH, state->ptr, state->end, tmp, 0))) {
				return NULL;
			}
			state->ptr += mb - 1;
		}
	} while (++state->ptr < state->end);

	done:
	 
	if (tmp != state->ptr) {
		state->buffer[state->offset++] = 0;
	} else {
		state->url.path = NULL;
	}
	return state->ptr;
}