long ext4_fallocate(struct inode *inode, int mode, loff_t offset, loff_t len)
{
	handle_t *handle;
	ext4_lblk_t block;
	loff_t new_size;
	unsigned int max_blocks;
	int ret = 0;
	int ret2 = 0;
	int retries = 0;
	struct buffer_head map_bh;
	unsigned int credits, blkbits = inode->i_blkbits;

	 
	if (!(EXT4_I(inode)->i_flags & EXT4_EXTENTS_FL))
		return -EOPNOTSUPP;

	 
	if (S_ISDIR(inode->i_mode))
		return -ENODEV;

	block = offset >> blkbits;
	 
	max_blocks = (EXT4_BLOCK_ALIGN(len + offset, blkbits) >> blkbits)
							- block;
	 
	credits = ext4_chunk_trans_blocks(inode, max_blocks);
	mutex_lock(&inode->i_mutex);
retry:
	while (ret >= 0 && ret < max_blocks) {
		block = block + ret;
		max_blocks = max_blocks - ret;
		handle = ext4_journal_start(inode, credits);
		if (IS_ERR(handle)) {
			ret = PTR_ERR(handle);
			break;
		}
		map_bh.b_state = 0;
		ret = ext4_get_blocks(handle, inode, block,
				      max_blocks, &map_bh,
				      EXT4_GET_BLOCKS_CREATE_UNINIT_EXT);
		if (ret <= 0) {
#ifdef EXT4FS_DEBUG
			WARN_ON(ret <= 0);
			printk(KERN_ERR "%s: ext4_ext_get_blocks "
				    "returned error inode#%lu, block=%u, "
				    "max_blocks=%u", __func__,
				    inode->i_ino, block, max_blocks);
#endif
			ext4_mark_inode_dirty(handle, inode);
			ret2 = ext4_journal_stop(handle);
			break;
		}
		if ((block + ret) >= (EXT4_BLOCK_ALIGN(offset + len,
						blkbits) >> blkbits))
			new_size = offset + len;
		else
			new_size = (block + ret) << blkbits;

		ext4_falloc_update_inode(inode, mode, new_size,
						buffer_new(&map_bh));
		ext4_mark_inode_dirty(handle, inode);
		ret2 = ext4_journal_stop(handle);
		if (ret2)
			break;
	}
	if (ret == -ENOSPC &&
			ext4_should_retry_alloc(inode->i_sb, &retries)) {
		ret = 0;
		goto retry;
	}
	mutex_unlock(&inode->i_mutex);
	return ret > 0 ? ret2 : ret;
}