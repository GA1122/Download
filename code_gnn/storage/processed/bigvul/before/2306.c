_PUBLIC_ int strcasecmp_m_handle(struct smb_iconv_handle *iconv_handle,
				 const char *s1, const char *s2)
{
	codepoint_t c1=0, c2=0;
	size_t size1, size2;

	 
	if (s1 == s2) return 0;
	if (s1 == NULL) return -1;
	if (s2 == NULL) return 1;

	while (*s1 && *s2) {
		c1 = next_codepoint_handle(iconv_handle, s1, &size1);
		c2 = next_codepoint_handle(iconv_handle, s2, &size2);

		if (c1 == INVALID_CODEPOINT ||
		    c2 == INVALID_CODEPOINT) {
			return strcasecmp(s1, s2);
		}

		s1 += size1;
		s2 += size2;

		if (c1 == c2) {
			continue;
		}

		if (toupper_m(c1) != toupper_m(c2)) {
			return c1 - c2;
		}
	}

	return *s1 - *s2;
}
