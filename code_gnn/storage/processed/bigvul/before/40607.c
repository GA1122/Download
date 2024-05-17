static char *alloc_one_pg_vec_page(unsigned long order)
{
	char *buffer = NULL;
	gfp_t gfp_flags = GFP_KERNEL | __GFP_COMP |
			  __GFP_ZERO | __GFP_NOWARN | __GFP_NORETRY;

	buffer = (char *) __get_free_pages(gfp_flags, order);

	if (buffer)
		return buffer;

	 
	buffer = vzalloc((1 << order) * PAGE_SIZE);

	if (buffer)
		return buffer;

	 
	gfp_flags &= ~__GFP_NORETRY;
	buffer = (char *)__get_free_pages(gfp_flags, order);
	if (buffer)
		return buffer;

	 
	return NULL;
}