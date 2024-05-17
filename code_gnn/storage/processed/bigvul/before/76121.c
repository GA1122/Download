cmd_str_r(const notify_script_t *script, char *buf, size_t len)
{
	char *str_p;
	int i;
	size_t str_len;

	str_p = buf;

	for (i = 0; i < script->num_args; i++) {
		 
		str_len = strlen(script->args[i]);
		if (str_p + str_len + 2 + (i ? 1 : 0) >= buf + len)
			return NULL;

		if (i)
			*str_p++ = ' ';
		*str_p++ = '\'';
		strcpy(str_p, script->args[i]);
		str_p += str_len;
		*str_p++ = '\'';
	}
	*str_p = '\0';

	return buf;
}