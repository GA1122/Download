ext4_ext_in_cache(struct inode *inode, ext4_lblk_t block,
			struct ext4_extent *ex)
{
	struct ext4_ext_cache *cex;
	int ret = EXT4_EXT_CACHE_NO;

	 
	spin_lock(&EXT4_I(inode)->i_block_reservation_lock);
	cex = &EXT4_I(inode)->i_cached_extent;

	 
	if (cex->ec_type == EXT4_EXT_CACHE_NO)
		goto errout;

	BUG_ON(cex->ec_type != EXT4_EXT_CACHE_GAP &&
			cex->ec_type != EXT4_EXT_CACHE_EXTENT);
	if (block >= cex->ec_block && block < cex->ec_block + cex->ec_len) {
		ex->ee_block = cpu_to_le32(cex->ec_block);
		ext4_ext_store_pblock(ex, cex->ec_start);
		ex->ee_len = cpu_to_le16(cex->ec_len);
		ext_debug("%u cached by %u:%u:%llu\n",
				block,
				cex->ec_block, cex->ec_len, cex->ec_start);
		ret = cex->ec_type;
	}
errout:
	spin_unlock(&EXT4_I(inode)->i_block_reservation_lock);
	return ret;
}