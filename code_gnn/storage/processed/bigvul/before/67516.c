int ext4_dax_mmap_get_block(struct inode *inode, sector_t iblock,
			    struct buffer_head *bh_result, int create)
{
	int ret, err;
	int credits;
	struct ext4_map_blocks map;
	handle_t *handle = NULL;
	int flags = 0;

	ext4_debug("ext4_dax_mmap_get_block: inode %lu, create flag %d\n",
		   inode->i_ino, create);
	map.m_lblk = iblock;
	map.m_len = bh_result->b_size >> inode->i_blkbits;
	credits = ext4_chunk_trans_blocks(inode, map.m_len);
	if (create) {
		flags |= EXT4_GET_BLOCKS_PRE_IO | EXT4_GET_BLOCKS_CREATE_ZERO;
		handle = ext4_journal_start(inode, EXT4_HT_MAP_BLOCKS, credits);
		if (IS_ERR(handle)) {
			ret = PTR_ERR(handle);
			return ret;
		}
	}

	ret = ext4_map_blocks(handle, inode, &map, flags);
	if (create) {
		err = ext4_journal_stop(handle);
		if (ret >= 0 && err < 0)
			ret = err;
	}
	if (ret <= 0)
		goto out;
	if (map.m_flags & EXT4_MAP_UNWRITTEN) {
		int err2;

		 
		handle = ext4_journal_start(inode, EXT4_HT_MAP_BLOCKS, credits);
		if (IS_ERR(handle)) {
			ret = PTR_ERR(handle);
			goto out;
		}

		err = ext4_map_blocks(handle, inode, &map,
		      EXT4_GET_BLOCKS_CONVERT | EXT4_GET_BLOCKS_CREATE_ZERO);
		if (err < 0)
			ret = err;
		err2 = ext4_journal_stop(handle);
		if (err2 < 0 && ret > 0)
			ret = err2;
	}
out:
	WARN_ON_ONCE(ret == 0 && create);
	if (ret > 0) {
		map_bh(bh_result, inode->i_sb, map.m_pblk);
		 
		map.m_flags &= ~EXT4_MAP_NEW;
		ext4_update_bh_state(bh_result, map.m_flags);
		bh_result->b_size = map.m_len << inode->i_blkbits;
		ret = 0;
	}
	return ret;
}