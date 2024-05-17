 static long ext4_zero_range(struct file *file, loff_t offset,
 			    loff_t len, int mode)
 {
 	struct inode *inode = file_inode(file);
 	handle_t *handle = NULL;
 	unsigned int max_blocks;
 	loff_t new_size = 0;
 	int ret = 0;
 	int flags;
 	int credits;
  	int partial_begin, partial_end;
  	loff_t start, end;
  	ext4_lblk_t lblk;
	struct address_space *mapping = inode->i_mapping;
  	unsigned int blkbits = inode->i_blkbits;
  
  	trace_ext4_zero_range(inode, offset, len, mode);
 
 	if (!S_ISREG(inode->i_mode))
 		return -EINVAL;
 
 	 
 	if (ext4_should_journal_data(inode)) {
 		ret = ext4_force_commit(inode->i_sb);
 		if (ret)
  			return ret;
  	}
  
	 
	if (mapping->nrpages && mapping_tagged(mapping, PAGECACHE_TAG_DIRTY)) {
		ret = filemap_write_and_wait_range(mapping, offset,
						   offset + len - 1);
		if (ret)
			return ret;
	}
  	 
 	start = round_up(offset, 1 << blkbits);
 	end = round_down((offset + len), 1 << blkbits);
 
 	if (start < offset || end > offset + len)
 		return -EINVAL;
 	partial_begin = offset & ((1 << blkbits) - 1);
 	partial_end = (offset + len) & ((1 << blkbits) - 1);
 
 	lblk = start >> blkbits;
 	max_blocks = (end >> blkbits);
 	if (max_blocks < lblk)
 		max_blocks = 0;
 	else
 		max_blocks -= lblk;
 
 	mutex_lock(&inode->i_mutex);
 
 	 
 	if (!(ext4_test_inode_flag(inode, EXT4_INODE_EXTENTS))) {
 		ret = -EOPNOTSUPP;
 		goto out_mutex;
 	}
 
 	if (!(mode & FALLOC_FL_KEEP_SIZE) &&
 	     offset + len > i_size_read(inode)) {
 		new_size = offset + len;
 		ret = inode_newsize_ok(inode, new_size);
 		if (ret)
 			goto out_mutex;
 	}
 
 	flags = EXT4_GET_BLOCKS_CREATE_UNWRIT_EXT;
 	if (mode & FALLOC_FL_KEEP_SIZE)
 		flags |= EXT4_GET_BLOCKS_KEEP_SIZE;
 
 	 
 	if (partial_begin || partial_end) {
 		ret = ext4_alloc_file_blocks(file,
 				round_down(offset, 1 << blkbits) >> blkbits,
 				(round_up((offset + len), 1 << blkbits) -
 				 round_down(offset, 1 << blkbits)) >> blkbits,
 				new_size, flags, mode);
 		if (ret)
 			goto out_mutex;
 
 	}
 
 	 
 	if (max_blocks > 0) {
  		flags |= (EXT4_GET_BLOCKS_CONVERT_UNWRITTEN |
  			  EXT4_EX_NOCACHE);
  
		 
		truncate_pagecache_range(inode, start, end - 1);
		inode->i_mtime = inode->i_ctime = ext4_current_time(inode);
  		 
  		ext4_inode_block_unlocked_dio(inode);
  		inode_dio_wait(inode);
  
// 		 
// 		down_write(&EXT4_I(inode)->i_mmap_sem);
// 		 
// 		truncate_pagecache_range(inode, start, end - 1);
// 		inode->i_mtime = inode->i_ctime = ext4_current_time(inode);
// 
  		ret = ext4_alloc_file_blocks(file, lblk, max_blocks, new_size,
  					     flags, mode);
// 		up_write(&EXT4_I(inode)->i_mmap_sem);
  		if (ret)
  			goto out_dio;
  	}
 	if (!partial_begin && !partial_end)
 		goto out_dio;
 
 	 
 	credits = (2 * ext4_ext_index_trans_blocks(inode, 2)) + 1;
 	if (ext4_should_journal_data(inode))
 		credits += 2;
 	handle = ext4_journal_start(inode, EXT4_HT_MISC, credits);
 	if (IS_ERR(handle)) {
 		ret = PTR_ERR(handle);
 		ext4_std_error(inode->i_sb, ret);
 		goto out_dio;
 	}
 
 	inode->i_mtime = inode->i_ctime = ext4_current_time(inode);
 	if (new_size) {
 		ext4_update_inode_size(inode, new_size);
 	} else {
 		 
 		if ((offset + len) > i_size_read(inode))
 			ext4_set_inode_flag(inode, EXT4_INODE_EOFBLOCKS);
 	}
 	ext4_mark_inode_dirty(handle, inode);
 
 	 
 	ret = ext4_zero_partial_blocks(handle, inode, offset, len);
 
 	if (file->f_flags & O_SYNC)
 		ext4_handle_sync(handle);
 
 	ext4_journal_stop(handle);
 out_dio:
 	ext4_inode_resume_unlocked_dio(inode);
 out_mutex:
 	mutex_unlock(&inode->i_mutex);
 	return ret;
 }