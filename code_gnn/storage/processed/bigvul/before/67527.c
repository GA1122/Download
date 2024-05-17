int ext4_get_block_unwritten(struct inode *inode, sector_t iblock,
			     struct buffer_head *bh_result, int create)
{
	ext4_debug("ext4_get_block_unwritten: inode %lu, create flag %d\n",
		   inode->i_ino, create);
	return _ext4_get_block(inode, iblock, bh_result,
			       EXT4_GET_BLOCKS_IO_CREATE_EXT);
}