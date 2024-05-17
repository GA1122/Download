int ext4_get_blocks(handle_t *handle, struct inode *inode, sector_t block,
		    unsigned int max_blocks, struct buffer_head *bh,
		    int flags)
{
	int retval;

	clear_buffer_mapped(bh);
	clear_buffer_unwritten(bh);

	ext_debug("ext4_get_blocks(): inode %lu, flag %d, max_blocks %u,"
		  "logical block %lu\n", inode->i_ino, flags, max_blocks,
		  (unsigned long)block);
	 
	down_read((&EXT4_I(inode)->i_data_sem));
	if (EXT4_I(inode)->i_flags & EXT4_EXTENTS_FL) {
		retval =  ext4_ext_get_blocks(handle, inode, block, max_blocks,
				bh, 0);
	} else {
		retval = ext4_ind_get_blocks(handle, inode, block, max_blocks,
					     bh, 0);
	}
	up_read((&EXT4_I(inode)->i_data_sem));

	if (retval > 0 && buffer_mapped(bh)) {
		int ret = check_block_validity(inode, "file system corruption",
					       block, bh->b_blocknr, retval);
		if (ret != 0)
			return ret;
	}

	 
	if ((flags & EXT4_GET_BLOCKS_CREATE) == 0)
		return retval;

	 
	if (retval > 0 && buffer_mapped(bh))
		return retval;

	 
	clear_buffer_unwritten(bh);

	 
	down_write((&EXT4_I(inode)->i_data_sem));

	 
	if (flags & EXT4_GET_BLOCKS_DELALLOC_RESERVE)
		EXT4_I(inode)->i_delalloc_reserved_flag = 1;
	 
	if (EXT4_I(inode)->i_flags & EXT4_EXTENTS_FL) {
		retval =  ext4_ext_get_blocks(handle, inode, block, max_blocks,
					      bh, flags);
	} else {
		retval = ext4_ind_get_blocks(handle, inode, block,
					     max_blocks, bh, flags);

		if (retval > 0 && buffer_new(bh)) {
			 
			ext4_clear_inode_state(inode, EXT4_STATE_EXT_MIGRATE);
		}

		 
		if ((retval > 0) &&
			(flags & EXT4_GET_BLOCKS_DELALLOC_RESERVE))
			ext4_da_update_reserve_space(inode, retval, 1);
	}
	if (flags & EXT4_GET_BLOCKS_DELALLOC_RESERVE)
		EXT4_I(inode)->i_delalloc_reserved_flag = 0;

	up_write((&EXT4_I(inode)->i_data_sem));
	if (retval > 0 && buffer_mapped(bh)) {
		int ret = check_block_validity(inode, "file system "
					       "corruption after allocation",
					       block, bh->b_blocknr, retval);
		if (ret != 0)
			return ret;
	}
	return retval;
}
