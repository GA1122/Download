int ext4_convert_unwritten_extents(struct inode *inode, loff_t offset,
				    ssize_t len)
{
	handle_t *handle;
	unsigned int max_blocks;
	int ret = 0;
	int ret2 = 0;
	struct ext4_map_blocks map;
	unsigned int credits, blkbits = inode->i_blkbits;

	map.m_lblk = offset >> blkbits;
	 
	max_blocks = ((EXT4_BLOCK_ALIGN(len + offset, blkbits) >> blkbits) -
		      map.m_lblk);
	 
	credits = ext4_chunk_trans_blocks(inode, max_blocks);
	while (ret >= 0 && ret < max_blocks) {
		map.m_lblk += ret;
		map.m_len = (max_blocks -= ret);
		handle = ext4_journal_start(inode, credits);
		if (IS_ERR(handle)) {
			ret = PTR_ERR(handle);
			break;
		}
		ret = ext4_map_blocks(handle, inode, &map,
				      EXT4_GET_BLOCKS_IO_CONVERT_EXT);
		if (ret <= 0) {
			WARN_ON(ret <= 0);
			ext4_msg(inode->i_sb, KERN_ERR,
				 "%s:%d: inode #%lu: block %u: len %u: "
				 "ext4_ext_map_blocks returned %d",
				 __func__, __LINE__, inode->i_ino, map.m_lblk,
				 map.m_len, ret);
		}
		ext4_mark_inode_dirty(handle, inode);
		ret2 = ext4_journal_stop(handle);
		if (ret <= 0 || ret2 )
			break;
	}
	return ret > 0 ? ret2 : ret;
}