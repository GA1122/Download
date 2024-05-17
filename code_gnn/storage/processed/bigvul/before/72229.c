parse_token(const char *cp, const char *filename,
	    int linenum, u_int *flags)
{
	u_int i;

	for (i = 0; keywords[i].name; i++)
		if (strcasecmp(cp, keywords[i].name) == 0) {
			*flags = keywords[i].flags;
			return keywords[i].opcode;
		}

	error("%s: line %d: Bad configuration option: %s",
	    filename, linenum, cp);
	return sBadOption;
}
