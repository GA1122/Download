ftrace_match_record(struct dyn_ftrace *rec, char *mod,
		    char *regex, int len, int type)
{
	char str[KSYM_SYMBOL_LEN];
	char *modname;

	kallsyms_lookup(rec->ip, NULL, NULL, &modname, str);

	if (mod) {
		 
		if (!modname || strcmp(modname, mod))
			return 0;

		 
		if (!len)
			return 1;
	}

	return ftrace_match(str, regex, len, type);
}
