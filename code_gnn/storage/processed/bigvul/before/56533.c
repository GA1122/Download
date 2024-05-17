static int __ext4_journalled_invalidatepage(struct page *page,
					    unsigned int offset,
					    unsigned int length)
{
	journal_t *journal = EXT4_JOURNAL(page->mapping->host);

	trace_ext4_journalled_invalidatepage(page, offset, length);

	 
	if (offset == 0 && length == PAGE_CACHE_SIZE)
		ClearPageChecked(page);

	return jbd2_journal_invalidatepage(journal, page, offset, length);
}