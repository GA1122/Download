  static void ext4_invalidatepage(struct page *page, unsigned long offset)
  {
  	journal_t *journal = EXT4_JOURNAL(page->mapping->host);
  
// 	 
// 	if (ext4_should_dioread_nolock(page->mapping->host))
// 		ext4_invalidatepage_free_endio(page, offset);
  	 
 	if (offset == 0)
 		ClearPageChecked(page);
 
 	if (journal)
 		jbd2_journal_invalidatepage(journal, page, offset);
 	else
 		block_invalidatepage(page, offset);
 }