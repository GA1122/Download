_PUBLIC_ char *strrchr_m(const char *s, char c)
{
	struct smb_iconv_handle *ic;
	char *ret = NULL;

	if (s == NULL) {
		return NULL;
	}

	 
	if ((c & 0xC0) == 0) {
		return strrchr(s, c);
	}

	 

	{
		size_t len = strlen(s);
		const char *cp = s;
		bool got_mb = false;

		if (len == 0)
			return NULL;
		cp += (len - 1);
		do {
			if (c == *cp) {
				 
				if ((cp > s) &&
					(((unsigned char)cp[-1]) & 0x80)) {
					 
					got_mb = true;
					break;
				}
				 
				return discard_const_p(char , cp);
			}
		} while (cp-- != s);
		if (!got_mb)
			return NULL;
	}

	ic = get_iconv_handle();

	while (*s) {
		size_t size;
		codepoint_t c2 = next_codepoint_handle(ic, s, &size);
		if (c2 == c) {
			ret = discard_const_p(char, s);
		}
		s += size;
	}

	return ret;
}
