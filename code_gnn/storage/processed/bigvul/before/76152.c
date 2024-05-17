decomment(char *str)
{
	bool quote = false;
	bool cont = false;
	char *skip = NULL;
	char *p = str + strspn(str, " \t");

	 
	if (p != str)
		memmove(str, p, strlen(p) + 1);

	p = str;
	while ((p = strpbrk(p, "!#\"\\"))) {
		if (*p == '"') {
			if (!skip)
				quote = !quote;
			p++;
			continue;
		}
		if (*p == '\\') {
			if (p[1]) {
				 
				if (!quote && (p[1] == '#' || p[1] == '!')) {
					memmove(p, p + 1, strlen(p + 1) + 1);
					p++;
				} else
					p += 2;
				continue;
			}
			*p = '\0';
			cont = true;
			break;
		}
		if (!quote && !skip && (*p == '!' || *p == '#'))
			skip = p;
		p++;
	}

	if (quote)
		report_config_error(CONFIG_GENERAL_ERROR, "Unterminated quote '%s'", str);

	if (skip)
		*skip = '\0';

	 
	p = str + strlen(str) - 1;
	while (p >= str && isblank(*p))
		*p-- = '\0';
	if (cont) {
		*++p = '\\';
		*++p = '\0';
	}
}