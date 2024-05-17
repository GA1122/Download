str_matches_option(const char *s1, const char *optionname)
{
	 
	size_t optlen = strlen(optionname);
	size_t slen = strlen(s1);
	if (slen == optlen || slen == optlen - 1)
		return !strncmp(s1, optionname, slen);
	else if (slen > optlen)
		return !strncmp(s1, optionname, optlen);
	else
		return 0;
}
