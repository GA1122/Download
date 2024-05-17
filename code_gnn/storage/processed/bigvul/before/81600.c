static int regex_match_full(char *str, struct regex *r, int len)
{
	 
	if (!len)
		return strcmp(str, r->pattern) == 0;

	return strncmp(str, r->pattern, len) == 0;
}
