static int move_to_new_page(struct page *newpage, struct page *page,
				enum migrate_mode mode)
{
	struct address_space *mapping;
	int rc;

	VM_BUG_ON_PAGE(!PageLocked(page), page);
	VM_BUG_ON_PAGE(!PageLocked(newpage), newpage);

	mapping = page_mapping(page);
	if (!mapping)
		rc = migrate_page(mapping, newpage, page, mode);
	else if (mapping->a_ops->migratepage)
		 
		rc = mapping->a_ops->migratepage(mapping, newpage, page, mode);
	else
		rc = fallback_migrate_page(mapping, newpage, page, mode);

	 
	if (rc == MIGRATEPAGE_SUCCESS) {
		set_page_memcg(page, NULL);
		if (!PageAnon(page))
			page->mapping = NULL;
	}
	return rc;
}
