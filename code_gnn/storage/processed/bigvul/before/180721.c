 static int ext4_writepage(struct page *page,
 			  struct writeback_control *wbc)
 {
  	int ret = 0;
  	loff_t size;
  	unsigned int len;
	struct buffer_head *page_bufs;
// 	struct buffer_head *page_bufs = NULL;
  	struct inode *inode = page->mapping->host;
  
  	trace_ext4_writepage(inode, page);
 	size = i_size_read(inode);
 	if (page->index == size >> PAGE_CACHE_SHIFT)
 		len = size & ~PAGE_CACHE_MASK;
 	else
 		len = PAGE_CACHE_SIZE;
 
 	if (page_has_buffers(page)) {
 		page_bufs = page_buffers(page);
 		if (walk_page_buffers(NULL, page_bufs, 0, len, NULL,
 					ext4_bh_delay_or_unwritten)) {
 			 
 			redirty_page_for_writepage(wbc, page);
 			unlock_page(page);
 			return 0;
 		}
 	} else {
 		 
 		ret = block_prepare_write(page, 0, len,
 					  noalloc_get_block_write);
 		if (!ret) {
 			page_bufs = page_buffers(page);
 			 
 			if (walk_page_buffers(NULL, page_bufs, 0, len, NULL,
 						ext4_bh_delay_or_unwritten)) {
 				redirty_page_for_writepage(wbc, page);
 				unlock_page(page);
 				return 0;
 			}
 		} else {
 			 
 			redirty_page_for_writepage(wbc, page);
 			unlock_page(page);
 			return 0;
 		}
 		 
 		block_commit_write(page, 0, len);
 	}
 
 	if (PageChecked(page) && ext4_should_journal_data(inode)) {
 		 
 		ClearPageChecked(page);
 		return __ext4_journalled_writepage(page, len);
 	}
  
  	if (test_opt(inode->i_sb, NOBH) && ext4_should_writeback_data(inode))
  		ret = nobh_writepage(page, noalloc_get_block_write, wbc);
	else
// 	else if (page_bufs && buffer_uninit(page_bufs)) {
// 		ext4_set_bh_endio(page_bufs, inode);
// 		ret = block_write_full_page_endio(page, noalloc_get_block_write,
// 					    wbc, ext4_end_io_buffer_write);
// 	} else
  		ret = block_write_full_page(page, noalloc_get_block_write,
  					    wbc);
  
 	return ret;
 }