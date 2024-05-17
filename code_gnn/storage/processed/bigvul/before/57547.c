int ext4_writepage_trans_blocks(struct inode *inode)
{
	int bpp = ext4_journal_blocks_per_page(inode);
	int ret;

	ret = ext4_meta_trans_blocks(inode, bpp, 0);

	 
	if (ext4_should_journal_data(inode))
		ret += bpp;
	return ret;
}
