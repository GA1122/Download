hotremove_migrate_alloc(struct page *page, unsigned long private, int **x)
{
	 
	return alloc_page(GFP_HIGHUSER_MOVABLE);
}
