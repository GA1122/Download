int nfs_migrate_page(struct address_space *mapping, struct page *newpage,
		struct page *page, enum migrate_mode mode)
{
	 
	if (PagePrivate(page))
		return -EBUSY;

	if (!nfs_fscache_release_page(page, GFP_KERNEL))
		return -EBUSY;

	return migrate_page(mapping, newpage, page, mode);
}
