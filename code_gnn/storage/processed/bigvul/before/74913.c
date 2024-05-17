_warc_rduri(const char *buf, size_t bsz)
{
	static const char _key[] = "\r\nWARC-Target-URI:";
	const char *val, *uri, *eol, *p;
	warc_string_t res = {0U, NULL};

	if ((val = xmemmem(buf, bsz, _key, sizeof(_key) - 1U)) == NULL) {
		 
		return res;
	}
	 
	val += sizeof(_key) - 1U;
	if ((eol = _warc_find_eol(val, buf + bsz - val)) == NULL) {
		 
		return res;
	}

	while (val < eol && (*val == ' ' || *val == '\t'))
		++val;

	 
	if ((uri = xmemmem(val, eol - val, "://", 3U)) == NULL) {
		 
		return res;
	}

	 
	for (p = val; p < eol; p++) {
		if (isspace((unsigned char)*p))
			return res;
	}

	 
	if (uri < (val + 3U))
		return res;

	 
	uri += 3U;

	 
	if (memcmp(val, "file", 4U) == 0) {
		 

	} else if (memcmp(val, "http", 4U) == 0 ||
		   memcmp(val, "ftp", 3U) == 0) {
		 
		while (uri < eol && *uri++ != '/');
	} else {
		 
		return res;
	}
	res.str = uri;
	res.len = eol - uri;
	return res;
}
