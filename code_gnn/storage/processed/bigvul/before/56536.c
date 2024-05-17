static int _ext4_get_block(struct inode *inode, sector_t iblock,
			   struct buffer_head *bh, int flags)
{
	handle_t *handle = ext4_journal_current_handle();
	struct ext4_map_blocks map;
	int ret = 0, started = 0;
	int dio_credits;

	if (ext4_has_inline_data(inode))
		return -ERANGE;

	map.m_lblk = iblock;
	map.m_len = bh->b_size >> inode->i_blkbits;

	if (flags && !(flags & EXT4_GET_BLOCKS_NO_LOCK) && !handle) {
		 
		if (map.m_len > DIO_MAX_BLOCKS)
			map.m_len = DIO_MAX_BLOCKS;
		dio_credits = ext4_chunk_trans_blocks(inode, map.m_len);
		handle = ext4_journal_start(inode, EXT4_HT_MAP_BLOCKS,
					    dio_credits);
		if (IS_ERR(handle)) {
			ret = PTR_ERR(handle);
			return ret;
		}
		started = 1;
	}

	ret = ext4_map_blocks(handle, inode, &map, flags);
	if (ret > 0) {
		ext4_io_end_t *io_end = ext4_inode_aio(inode);

		map_bh(bh, inode->i_sb, map.m_pblk);
		bh->b_state = (bh->b_state & ~EXT4_MAP_FLAGS) | map.m_flags;
		if (IS_DAX(inode) && buffer_unwritten(bh)) {
			 
			WARN_ON_ONCE(io_end);
			bh->b_assoc_map = inode->i_mapping;
			bh->b_private = (void *)(unsigned long)iblock;
		}
		if (io_end && io_end->flag & EXT4_IO_END_UNWRITTEN)
			set_buffer_defer_completion(bh);
		bh->b_size = inode->i_sb->s_blocksize * map.m_len;
		ret = 0;
	}
	if (started)
		ext4_journal_stop(handle);
	return ret;
}