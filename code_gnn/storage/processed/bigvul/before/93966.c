xfs_start_page_writeback(
	struct page		*page,
	int			clear_dirty)
{
	ASSERT(PageLocked(page));
	ASSERT(!PageWriteback(page));

	 
	if (clear_dirty) {
		clear_page_dirty_for_io(page);
		set_page_writeback(page);
	} else
		set_page_writeback_keepwrite(page);

	unlock_page(page);
}
