bid_entry(const char *p, ssize_t len, ssize_t nl, int *last_is_path)
{
	int f = 0;
	static const unsigned char safe_char[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		 
		0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
		 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,  
		 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
		 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
		 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
		 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
	};
	ssize_t ll;
	const char *pp = p;
	const char * const pp_end = pp + len;

	*last_is_path = 0;
	 
	for (;pp < pp_end; ++pp) {
		if (!safe_char[*(const unsigned char *)pp]) {
			if (*pp != ' ' && *pp != '\t' && *pp != '\r'
			    && *pp != '\n')
				f = 0;
			break;
		}
		f = 1;
	}
	ll = pp_end - pp;

	 
	if (f == 0) {
		const char *pb = p + len - nl;
		int name_len = 0;
		int slash;

		 
		if (pb-2 >= p &&
		    pb[-1] == '\\' && (pb[-2] == ' ' || pb[-2] == '\t'))
			return (-1);
		if (pb-1 >= p && pb[-1] == '\\')
			return (-1);

		slash = 0;
		while (p <= --pb && *pb != ' ' && *pb != '\t') {
			if (!safe_char[*(const unsigned char *)pb])
				return (-1);
			name_len++;
			 
			if (*pb == '/')
				slash = 1;
		}
		if (name_len == 0 || slash == 0)
			return (-1);
		 
		if (pb[1] == '/')
			return (-1);
		ll = len - nl - name_len;
		pp = p;
		*last_is_path = 1;
	}

	return (bid_keyword_list(pp, ll, 0, *last_is_path));
}
