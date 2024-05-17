parse_snapshot(const char *str)
{
	txid		xmin;
	txid		xmax;
	txid		last_val = 0,
				val;
	const char *str_start = str;
	const char *endp;
	StringInfo	buf;

	xmin = str2txid(str, &endp);
	if (*endp != ':')
		goto bad_format;
	str = endp + 1;

	xmax = str2txid(str, &endp);
	if (*endp != ':')
		goto bad_format;
	str = endp + 1;

	 
	if (xmin == 0 || xmax == 0 || xmin > xmax)
		goto bad_format;

	 
	buf = buf_init(xmin, xmax);

	 
	while (*str != '\0')
	{
		 
		val = str2txid(str, &endp);
		str = endp;

		 
		if (val < xmin || val >= xmax || val <= last_val)
			goto bad_format;

		buf_add_txid(buf, val);
		last_val = val;

		if (*str == ',')
			str++;
		else if (*str != '\0')
			goto bad_format;
	}

	return buf_finalize(buf);

bad_format:
	elog(ERROR, "invalid input for txid_snapshot: \"%s\"", str_start);
	return NULL;
}
