static int noalloc_get_block_write(struct inode *inode, sector_t iblock,
				   struct buffer_head *bh_result, int create)
{
	int ret = 0;
	unsigned max_blocks = bh_result->b_size >> inode->i_blkbits;

	BUG_ON(bh_result->b_size != inode->i_sb->s_blocksize);

	 
	ret = ext4_get_blocks(NULL, inode, iblock, max_blocks, bh_result, 0);
	if (ret > 0) {
		bh_result->b_size = (ret << inode->i_blkbits);
		ret = 0;
	}
	return ret;
}