static loff_t max_file_blocks(void)
{
	loff_t result = (DEF_ADDRS_PER_INODE - F2FS_INLINE_XATTR_ADDRS);
	loff_t leaf_count = ADDRS_PER_BLOCK;

	 
	result += (leaf_count * 2);

	 
	leaf_count *= NIDS_PER_BLOCK;
	result += (leaf_count * 2);

	 
	leaf_count *= NIDS_PER_BLOCK;
	result += leaf_count;

	return result;
}
