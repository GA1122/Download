static size_t log_prefix(const char *p, unsigned int *level, char *special)
{
	unsigned int lev = 0;
	char sp = '\0';
	size_t len;

	if (p[0] != '<' || !p[1])
		return 0;
	if (p[2] == '>') {
		 
		switch (p[1]) {
		case '0' ... '7':
			lev = p[1] - '0';
			break;
		case 'c':  
		case 'd':  
			sp = p[1];
			break;
		default:
			return 0;
		}
		len = 3;
	} else {
		 
		char *endp = NULL;

		lev = (simple_strtoul(&p[1], &endp, 10) & 7);
		if (endp == NULL || endp[0] != '>')
			return 0;
		len = (endp + 1) - p;
	}

	 
	if (sp && !special)
		return 0;

	if (special) {
		*special = sp;
		 
		if (sp)
			return len;
	}

	if (level)
		*level = lev;
	return len;
}
