static int ext4_da_write_credits(struct inode *inode, loff_t pos, unsigned len)
{
	if (likely(ext4_has_feature_large_file(inode->i_sb)))
		return 1;

	if (pos + len <= 0x7fffffffULL)
		return 1;

	 
	return 2;
}