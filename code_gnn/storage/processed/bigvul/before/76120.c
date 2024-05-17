cmd_str(const notify_script_t *script)
{
	size_t len;
	int i;

	for (i = 0, len = 0; i < script->num_args; i++)
		len += strlen(script->args[i]) + 3;  

	if (len > sizeof cmd_str_buf)
		return NULL;

	return cmd_str_r(script, cmd_str_buf, sizeof cmd_str_buf);
}
