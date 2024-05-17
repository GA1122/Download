static int fuse_writepage(struct page *page, struct writeback_control *wbc)
{
	int err;

	if (fuse_page_is_writeback(page->mapping->host, page->index)) {
		 
		WARN_ON(wbc->sync_mode == WB_SYNC_ALL);

		redirty_page_for_writepage(wbc, page);
		return 0;
	}

	err = fuse_writepage_locked(page);
	unlock_page(page);

	return err;
}
