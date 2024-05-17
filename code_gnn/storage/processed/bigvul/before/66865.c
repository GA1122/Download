static const char *page_size_string(struct map_range *mr)
{
	static const char str_1g[] = "1G";
	static const char str_2m[] = "2M";
	static const char str_4m[] = "4M";
	static const char str_4k[] = "4k";

	if (mr->page_size_mask & (1<<PG_LEVEL_1G))
		return str_1g;
	 
	if (IS_ENABLED(CONFIG_X86_32) &&
	    !IS_ENABLED(CONFIG_X86_PAE) &&
	    mr->page_size_mask & (1<<PG_LEVEL_2M))
		return str_4m;

	if (mr->page_size_mask & (1<<PG_LEVEL_2M))
		return str_2m;

	return str_4k;
}
