int migrate_page(struct address_space *mapping,
		struct page *newpage, struct page *page,
		enum migrate_mode mode)
{
	int rc;

	BUG_ON(PageWriteback(page));	 

	rc = migrate_page_move_mapping(mapping, newpage, page, NULL, mode, 0);

	if (rc != MIGRATEPAGE_SUCCESS)
		return rc;

	migrate_page_copy(newpage, page);
	return MIGRATEPAGE_SUCCESS;
}