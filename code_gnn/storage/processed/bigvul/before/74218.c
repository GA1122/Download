leap_file(
	FILE	*fp		 
	)
{
	char	buf[NTP_MAXSTRLEN];  
	u_long	leap;		 
	u_long	expire;		 
	int	offset;		 
	int	i;

	 
	offset = 0;
	leap = 0;
	expire = 0;
	i = 10;
	while (fgets(buf, NTP_MAXSTRLEN - 1, fp) != NULL) {
		if (strlen(buf) < 1)
			continue;

		if (buf[0] == '#') {
			if (strlen(buf) < 3)
				continue;

			 
			if (buf[1] == '@' || buf[1] == '$') {
				if (sscanf(&buf[2], "%lu", &expire) !=
				    1)
					return (-1);

				continue;
			}
		}
		if (sscanf(buf, "%lu %d", &leap, &offset) == 2) {

			 
			if (i++ != offset)
				return (-1);
		}
	}

	 
	if (i == 10)
		return (-1);

	leap_tai = offset;
	leap_sec = leap;
	leap_expire = expire;
	return (0);
}
