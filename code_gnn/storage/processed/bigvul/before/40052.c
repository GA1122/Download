DecodeTimezone(char *str, int *tzp)
{
	int			tz;
	int			hr,
				min;
	char	   *cp;
	int			len;

	 
	hr = strtol(str + 1, &cp, 10);

	 
	if (*cp == ':')
		min = strtol(cp + 1, &cp, 10);
	 
	else if (*cp == '\0' && (len = strlen(str)) > 3)
	{
		min = strtol(str + len - 2, &cp, 10);
		if (min < 0 || min >= 60)
			return -1;

		*(str + len - 2) = '\0';
		hr = strtol(str + 1, &cp, 10);
		if (hr < 0 || hr > 13)
			return -1;
	}
	else
		min = 0;

	tz = (hr * MINS_PER_HOUR + min) * SECS_PER_MINUTE;
	if (*str == '-')
		tz = -tz;

	*tzp = -tz;
	return *cp != '\0';
}	 