char *strstr_m(const char *src, const char *findstr)
{
	smb_ucs2_t *p;
	smb_ucs2_t *src_w, *find_w;
	const char *s;
	char *s2;
	char *retp;
	size_t converted_size, findstr_len = 0;

	TALLOC_CTX *frame;  

	 
	if (!findstr[0]) {
		return discard_const_p(char, src);
	}

	 
	if (findstr[1] == '\0')
		return strchr_m(src, *findstr);

	 

	for (s = src; *s && !(((unsigned char)s[0]) & 0x80); s++) {
		if (*s == *findstr) {
			if (!findstr_len)
				findstr_len = strlen(findstr);

			if (strncmp(s, findstr, findstr_len) == 0) {
				return discard_const_p(char, s);
			}
		}
	}

	if (!*s)
		return NULL;

#if 1  
	 

	 
	s = src;
#endif

	frame = talloc_stackframe();

	if (!push_ucs2_talloc(frame, &src_w, src, &converted_size)) {
		DBG_WARNING("src malloc fail\n");
		TALLOC_FREE(frame);
		return NULL;
	}

	if (!push_ucs2_talloc(frame, &find_w, findstr, &converted_size)) {
		DBG_WARNING("find malloc fail\n");
		TALLOC_FREE(frame);
		return NULL;
	}

	p = strstr_w(src_w, find_w);

	if (!p) {
		TALLOC_FREE(frame);
		return NULL;
	}

	*p = 0;
	if (!pull_ucs2_talloc(frame, &s2, src_w, &converted_size)) {
		TALLOC_FREE(frame);
		DEBUG(0,("strstr_m: dest malloc fail\n"));
		return NULL;
	}
	retp = discard_const_p(char, (s+strlen(s2)));
	TALLOC_FREE(frame);
	return retp;
}
