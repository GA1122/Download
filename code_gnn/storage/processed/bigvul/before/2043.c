static int parse_cred_line(char *line, char **target)
{
	if (line == NULL || target == NULL)
		goto parsing_err;

	 
	*target = strchr(line, '=');
	if (!*target)
		goto parsing_err;
	*target += 1;

	 
	if (strncasecmp("user", line, 4) == 0)
		return CRED_USER;
	if (strncasecmp("pass", line, 4) == 0)
		return CRED_PASS;
	if (strncasecmp("dom", line, 3) == 0)
		return CRED_DOM;

parsing_err:
	return CRED_UNPARSEABLE;
}
