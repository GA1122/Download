int buffer_migrate_page(struct address_space *mapping,
		struct page *newpage, struct page *page, enum migrate_mode mode)
{
	struct buffer_head *bh, *head;
	int rc;

	if (!page_has_buffers(page))
		return migrate_page(mapping, newpage, page, mode);

	head = page_buffers(page);

	rc = migrate_page_move_mapping(mapping, newpage, page, head, mode, 0);

	if (rc != MIGRATEPAGE_SUCCESS)
		return rc;

	 
	if (mode != MIGRATE_ASYNC)
		BUG_ON(!buffer_migrate_lock_buffers(head, mode));

	ClearPagePrivate(page);
	set_page_private(newpage, page_private(page));
	set_page_private(page, 0);
	put_page(page);
	get_page(newpage);

	bh = head;
	do {
		set_bh_page(bh, newpage, bh_offset(bh));
		bh = bh->b_this_page;

	} while (bh != head);

	SetPagePrivate(newpage);

	migrate_page_copy(newpage, page);

	bh = head;
	do {
		unlock_buffer(bh);
 		put_bh(bh);
		bh = bh->b_this_page;

	} while (bh != head);

	return MIGRATEPAGE_SUCCESS;
}