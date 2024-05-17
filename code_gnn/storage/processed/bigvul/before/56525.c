static void ext4_end_io_unwritten(struct buffer_head *bh, int uptodate)
{
	struct inode *inode = bh->b_assoc_map->host;
	 
	loff_t offset = (loff_t)(uintptr_t)bh->b_private << inode->i_blkbits;
	int err;
	if (!uptodate)
		return;
	WARN_ON(!buffer_unwritten(bh));
	err = ext4_convert_unwritten_extents(NULL, inode, offset, bh->b_size);
}