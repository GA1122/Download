static char *gather_options_str(char *opts, const char *append)
{
	 
	if (append) {
		if (opts == NULL) {
			opts = xstrdup(append);
		} else {
			int optlen = strlen(opts);
			opts = xrealloc(opts, optlen + strlen(append) + 2);
			sprintf(opts + optlen, " %s", append);
		}
	}
	return opts;
}