void free_argv(void)
{
	while (newargc)
		free(newargv[--newargc]);
	while (oldargc)
		free(oldargv[--oldargc]);
}
