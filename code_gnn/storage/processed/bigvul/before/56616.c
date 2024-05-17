int ext4_zero_partial_blocks(handle_t *handle, struct inode *inode,
			     loff_t lstart, loff_t length)
{
	struct super_block *sb = inode->i_sb;
	struct address_space *mapping = inode->i_mapping;
	unsigned partial_start, partial_end;
	ext4_fsblk_t start, end;
	loff_t byte_end = (lstart + length - 1);
	int err = 0;

	partial_start = lstart & (sb->s_blocksize - 1);
	partial_end = byte_end & (sb->s_blocksize - 1);

	start = lstart >> sb->s_blocksize_bits;
	end = byte_end >> sb->s_blocksize_bits;

	 
	if (start == end &&
	    (partial_start || (partial_end != sb->s_blocksize - 1))) {
		err = ext4_block_zero_page_range(handle, mapping,
						 lstart, length);
		return err;
	}
	 
	if (partial_start) {
		err = ext4_block_zero_page_range(handle, mapping,
						 lstart, sb->s_blocksize);
		if (err)
			return err;
	}
	 
	if (partial_end != sb->s_blocksize - 1)
		err = ext4_block_zero_page_range(handle, mapping,
						 byte_end - partial_end,
						 partial_end + 1);
	return err;
}