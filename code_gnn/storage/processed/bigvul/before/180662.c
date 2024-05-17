 int ext4_punch_hole(struct inode *inode, loff_t offset, loff_t length)
 {
 	struct super_block *sb = inode->i_sb;
 	ext4_lblk_t first_block, stop_block;
 	struct address_space *mapping = inode->i_mapping;
 	loff_t first_block_offset, last_block_offset;
 	handle_t *handle;
 	unsigned int credits;
 	int ret = 0;
 
 	if (!S_ISREG(inode->i_mode))
 		return -EOPNOTSUPP;
 
 	trace_ext4_punch_hole(inode, offset, length, 0);
 
 	 
 	if (mapping->nrpages && mapping_tagged(mapping, PAGECACHE_TAG_DIRTY)) {
 		ret = filemap_write_and_wait_range(mapping, offset,
 						   offset + length - 1);
 		if (ret)
 			return ret;
 	}
 
 	mutex_lock(&inode->i_mutex);
 
 	 
 	if (offset >= inode->i_size)
 		goto out_mutex;
 
 	 
 	if (offset + length > inode->i_size) {
 		length = inode->i_size +
 		   PAGE_CACHE_SIZE - (inode->i_size & (PAGE_CACHE_SIZE - 1)) -
 		   offset;
 	}
 
 	if (offset & (sb->s_blocksize - 1) ||
 	    (offset + length) & (sb->s_blocksize - 1)) {
 		 
 		ret = ext4_inode_attach_jinode(inode);
 		if (ret < 0)
 			goto out_mutex;
  
  	}
  
// 	 
// 	ext4_inode_block_unlocked_dio(inode);
// 	inode_dio_wait(inode);
// 
// 	 
// 	down_write(&EXT4_I(inode)->i_mmap_sem);
  	first_block_offset = round_up(offset, sb->s_blocksize);
  	last_block_offset = round_down((offset + length), sb->s_blocksize) - 1;
  
 	 
 	if (last_block_offset > first_block_offset)
  		truncate_pagecache_range(inode, first_block_offset,
  					 last_block_offset);
  
	 
	ext4_inode_block_unlocked_dio(inode);
	inode_dio_wait(inode);
  	if (ext4_test_inode_flag(inode, EXT4_INODE_EXTENTS))
  		credits = ext4_writepage_trans_blocks(inode);
  	else
 		credits = ext4_blocks_for_truncate(inode);
 	handle = ext4_journal_start(inode, EXT4_HT_TRUNCATE, credits);
 	if (IS_ERR(handle)) {
 		ret = PTR_ERR(handle);
 		ext4_std_error(sb, ret);
 		goto out_dio;
 	}
 
 	ret = ext4_zero_partial_blocks(handle, inode, offset,
 				       length);
 	if (ret)
 		goto out_stop;
 
 	first_block = (offset + sb->s_blocksize - 1) >>
 		EXT4_BLOCK_SIZE_BITS(sb);
 	stop_block = (offset + length) >> EXT4_BLOCK_SIZE_BITS(sb);
 
 	 
 	if (first_block >= stop_block)
 		goto out_stop;
 
 	down_write(&EXT4_I(inode)->i_data_sem);
 	ext4_discard_preallocations(inode);
 
 	ret = ext4_es_remove_extent(inode, first_block,
 				    stop_block - first_block);
 	if (ret) {
 		up_write(&EXT4_I(inode)->i_data_sem);
 		goto out_stop;
 	}
 
 	if (ext4_test_inode_flag(inode, EXT4_INODE_EXTENTS))
 		ret = ext4_ext_remove_space(inode, first_block,
 					    stop_block - 1);
 	else
 		ret = ext4_ind_remove_space(handle, inode, first_block,
 					    stop_block);
 
 	up_write(&EXT4_I(inode)->i_data_sem);
  	if (IS_SYNC(inode))
  		ext4_handle_sync(handle);
  
	 
	if (last_block_offset > first_block_offset)
		truncate_pagecache_range(inode, first_block_offset,
					 last_block_offset);
  	inode->i_mtime = inode->i_ctime = ext4_current_time(inode);
  	ext4_mark_inode_dirty(handle, inode);
  out_stop:
  	ext4_journal_stop(handle);
  out_dio:
// 	up_write(&EXT4_I(inode)->i_mmap_sem);
  	ext4_inode_resume_unlocked_dio(inode);
  out_mutex:
  	mutex_unlock(&inode->i_mutex);
 	return ret;
 }