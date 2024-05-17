xstrpisotime(const char *s, char **endptr)
{
 
	struct tm tm;
	time_t res = (time_t)-1;

	 
	memset(&tm, 0, sizeof(tm));

	 
	while (*s == ' ' || *s == '\t')
		++s;

	 
	if ((tm.tm_year = strtoi_lim(s, &s, 1583, 4095)) < 0 || *s++ != '-') {
		goto out;
	}
	 
	if ((tm.tm_mon = strtoi_lim(s, &s, 1, 12)) < 0 || *s++ != '-') {
		goto out;
	}
	 
	if ((tm.tm_mday = strtoi_lim(s, &s, 1, 31)) < 0 || *s++ != 'T') {
		goto out;
	}
	 
	if ((tm.tm_hour = strtoi_lim(s, &s, 0, 23)) < 0 || *s++ != ':') {
		goto out;
	}
	 
	if ((tm.tm_min = strtoi_lim(s, &s, 0, 59)) < 0 || *s++ != ':') {
		goto out;
	}
	 
	if ((tm.tm_sec = strtoi_lim(s, &s, 0, 60)) < 0 || *s++ != 'Z') {
		goto out;
	}

	 
	tm.tm_year -= 1900;
	tm.tm_mon--;

	 
	res = time_from_tm(&tm);

out:
	if (endptr != NULL) {
		*endptr = deconst(s);
	}
	return res;
}
