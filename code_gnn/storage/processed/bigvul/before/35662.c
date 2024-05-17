print_sep(struct magic_set *ms, int firstline)
{
	if (ms->flags & MAGIC_MIME)
		return 0;
	if (firstline)
		return 0;
	 
	return file_printf(ms, "\n- ");
}
