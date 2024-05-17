static int ext4_block_zero_page_range(handle_t *handle,
		struct address_space *mapping, loff_t from, loff_t length)
{
	struct inode *inode = mapping->host;
	unsigned offset = from & (PAGE_SIZE-1);
	unsigned blocksize = inode->i_sb->s_blocksize;
	unsigned max = blocksize - (offset & (blocksize - 1));

	 
	if (length > max || length < 0)
		length = max;

	if (IS_DAX(inode))
		return dax_zero_page_range(inode, from, length, ext4_get_block);
	return __ext4_block_zero_page_range(handle, mapping, from, length);
}