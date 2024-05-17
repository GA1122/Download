ftrace_mod_callback(struct ftrace_hash *hash,
		    char *func, char *cmd, char *param, int enable)
{
	char *mod;
	int ret = -EINVAL;

	 

	 
	if (!param)
		return ret;

	mod = strsep(&param, ":");
	if (!strlen(mod))
		return ret;

	ret = ftrace_match_module_records(hash, func, mod);
	if (!ret)
		ret = -EINVAL;
	if (ret < 0)
		return ret;

	return 0;
}
