parse_range_spec(char *str, size_t size, struct range *r)
{
	size_t		 start_str_len, end_str_len;
	char		*p, *start_str, *end_str;
	const char	*errstr;

	if ((p = strchr(str, '-')) == NULL)
		return (0);

	*p++ = '\0';
	start_str = str;
	end_str = p;
	start_str_len = strlen(start_str);
	end_str_len = strlen(end_str);

	 
	if ((start_str_len == 0) && (end_str_len == 0))
		return (0);

	if (end_str_len) {
		r->end = strtonum(end_str, 0, LLONG_MAX, &errstr);
		if (errstr)
			return (0);

		if ((size_t)r->end >= size)
			r->end = size - 1;
	} else
		r->end = size - 1;

	if (start_str_len) {
		r->start = strtonum(start_str, 0, LLONG_MAX, &errstr);
		if (errstr)
			return (0);

		if ((size_t)r->start >= size)
			return (0);
	} else {
		r->start = size - r->end;
		r->end = size - 1;
	}

	if (r->end < r->start)
		return (0);
 
 	return (1);
 }