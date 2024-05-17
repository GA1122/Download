nonmagic(const char *str)
{
	const char *p;
	size_t rv = 0;

	for (p = str; *p; p++)
		switch (*p) {
		case '\\':	 
			if (!*++p)
				p--;
			rv++;
			continue;
		case '?':	 
		case '*':
		case '.':
		case '+':
		case '^':
		case '$':
			continue;
		case '[':	 
			while (*p && *p != ']')
				p++;
			p--;
			continue;
		case '{':	 
			while (*p && *p != '}')
				p++;
			if (!*p)
				p--;
			continue;
		default:	 
			rv++;
			continue;
		}

	return rv == 0 ? 1 : rv;	 
}
