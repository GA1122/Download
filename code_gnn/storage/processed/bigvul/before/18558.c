ext4_ext_in_cache(struct inode *inode, ext4_lblk_t block,
		  struct ext4_extent *ex)
{
	struct ext4_ext_cache *cex;
	struct ext4_sb_info *sbi;
	int ret = 0;

	 
	spin_lock(&EXT4_I(inode)->i_block_reservation_lock);
	cex = &EXT4_I(inode)->i_cached_extent;
	sbi = EXT4_SB(inode->i_sb);

	 
	if (cex->ec_len == 0)
		goto errout;

	if (in_range(block, cex->ec_block, cex->ec_len)) {
		ex->ee_block = cpu_to_le32(cex->ec_block);
		ext4_ext_store_pblock(ex, cex->ec_start);
		ex->ee_len = cpu_to_le16(cex->ec_len);
		ext_debug("%u cached by %u:%u:%llu\n",
				block,
				cex->ec_block, cex->ec_len, cex->ec_start);
		ret = 1;
	}
errout:
	trace_ext4_ext_in_cache(inode, block, ret);
	spin_unlock(&EXT4_I(inode)->i_block_reservation_lock);
	return ret;
}