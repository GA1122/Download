struct buffer_head *ext4_getblk(handle_t *handle, struct inode *inode,
				ext4_lblk_t block, int map_flags)
{
	struct ext4_map_blocks map;
	struct buffer_head *bh;
	int create = map_flags & EXT4_GET_BLOCKS_CREATE;
	int err;

	J_ASSERT(handle != NULL || create == 0);

	map.m_lblk = block;
	map.m_len = 1;
	err = ext4_map_blocks(handle, inode, &map, map_flags);

	if (err == 0)
		return create ? ERR_PTR(-ENOSPC) : NULL;
	if (err < 0)
		return ERR_PTR(err);

	bh = sb_getblk(inode->i_sb, map.m_pblk);
	if (unlikely(!bh))
		return ERR_PTR(-ENOMEM);
	if (map.m_flags & EXT4_MAP_NEW) {
		J_ASSERT(create != 0);
		J_ASSERT(handle != NULL);

		 
		lock_buffer(bh);
		BUFFER_TRACE(bh, "call get_create_access");
		err = ext4_journal_get_create_access(handle, bh);
		if (unlikely(err)) {
			unlock_buffer(bh);
			goto errout;
		}
		if (!buffer_uptodate(bh)) {
			memset(bh->b_data, 0, inode->i_sb->s_blocksize);
			set_buffer_uptodate(bh);
		}
		unlock_buffer(bh);
		BUFFER_TRACE(bh, "call ext4_handle_dirty_metadata");
		err = ext4_handle_dirty_metadata(handle, inode, bh);
		if (unlikely(err))
			goto errout;
	} else
		BUFFER_TRACE(bh, "not a new buffer");
	return bh;
errout:
	brelse(bh);
	return ERR_PTR(err);
}