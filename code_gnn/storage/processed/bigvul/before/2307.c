_PUBLIC_ char *strchr_m(const char *src, char c)
{
	const char *s;
	struct smb_iconv_handle *ic = get_iconv_handle();
	if (src == NULL) {
		return NULL;
	}
	 
	if ((c & 0xC0) == 0) {
		return strchr(src, c);
	}

	 

	for (s = src; *s && !(((unsigned char)s[0]) & 0x80); s++) {
		if (*s == c)
			return discard_const_p(char, s);
	}

	if (!*s)
		return NULL;

#ifdef BROKEN_UNICODE_COMPOSE_CHARACTERS
	 
	s = src;
#endif

	while (*s) {
		size_t size;
		codepoint_t c2 = next_codepoint_handle(ic, s, &size);
		if (c2 == c) {
			return discard_const_p(char, s);
		}
		s += size;
	}

	return NULL;
}
