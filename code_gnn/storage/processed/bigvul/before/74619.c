static int parse_start(char **cp)
{
	char *p;

	p = strip_line(*cp);

	 
	if (*p == '\0' || *p == '#')
		return 0;

	if (!strncmp(p, "<device", 7)) {
		DBG(READ, ul_debug("found device header: %8s", p));
		p += 7;

		*cp = p;
		return 1;
	}

	if (*p == '<')
		return 0;

	return -BLKID_ERR_CACHE;
}