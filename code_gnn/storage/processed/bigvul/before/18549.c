static int ext4_ext_convert_to_initialized(handle_t *handle,
					   struct inode *inode,
					   struct ext4_map_blocks *map,
					   struct ext4_ext_path *path)
{
	struct ext4_sb_info *sbi;
	struct ext4_extent_header *eh;
	struct ext4_map_blocks split_map;
	struct ext4_extent zero_ex;
	struct ext4_extent *ex;
	ext4_lblk_t ee_block, eof_block;
	unsigned int ee_len, depth;
	int allocated, max_zeroout = 0;
	int err = 0;
	int split_flag = 0;

	ext_debug("ext4_ext_convert_to_initialized: inode %lu, logical"
		"block %llu, max_blocks %u\n", inode->i_ino,
		(unsigned long long)map->m_lblk, map->m_len);

	sbi = EXT4_SB(inode->i_sb);
	eof_block = (inode->i_size + inode->i_sb->s_blocksize - 1) >>
		inode->i_sb->s_blocksize_bits;
	if (eof_block < map->m_lblk + map->m_len)
		eof_block = map->m_lblk + map->m_len;

	depth = ext_depth(inode);
	eh = path[depth].p_hdr;
	ex = path[depth].p_ext;
	ee_block = le32_to_cpu(ex->ee_block);
	ee_len = ext4_ext_get_actual_len(ex);
	allocated = ee_len - (map->m_lblk - ee_block);

	trace_ext4_ext_convert_to_initialized_enter(inode, map, ex);

	 
	BUG_ON(!ext4_ext_is_uninitialized(ex));
	BUG_ON(!in_range(map->m_lblk, ee_block, ee_len));

	 
	if ((map->m_lblk == ee_block) &&	 
		(map->m_len < ee_len) &&	 
		(ex > EXT_FIRST_EXTENT(eh))) {	 
		struct ext4_extent *prev_ex;
		ext4_lblk_t prev_lblk;
		ext4_fsblk_t prev_pblk, ee_pblk;
		unsigned int prev_len, write_len;

		prev_ex = ex - 1;
		prev_lblk = le32_to_cpu(prev_ex->ee_block);
		prev_len = ext4_ext_get_actual_len(prev_ex);
		prev_pblk = ext4_ext_pblock(prev_ex);
		ee_pblk = ext4_ext_pblock(ex);
		write_len = map->m_len;

		 
		if ((!ext4_ext_is_uninitialized(prev_ex)) &&		 
			((prev_lblk + prev_len) == ee_block) &&		 
			((prev_pblk + prev_len) == ee_pblk) &&		 
			(prev_len < (EXT_INIT_MAX_LEN - write_len))) {	 
			err = ext4_ext_get_access(handle, inode, path + depth);
			if (err)
				goto out;

			trace_ext4_ext_convert_to_initialized_fastpath(inode,
				map, ex, prev_ex);

			 
			ex->ee_block = cpu_to_le32(ee_block + write_len);
			ext4_ext_store_pblock(ex, ee_pblk + write_len);
			ex->ee_len = cpu_to_le16(ee_len - write_len);
			ext4_ext_mark_uninitialized(ex);  

			 
			prev_ex->ee_len = cpu_to_le16(prev_len + write_len);

			 
			ext4_ext_dirty(handle, inode, path + depth);

			 
			path[depth].p_ext = prev_ex;

			 
			allocated = write_len;
			goto out;
		}
	}

	WARN_ON(map->m_lblk < ee_block);
	 
	split_flag |= ee_block + ee_len <= eof_block ? EXT4_EXT_MAY_ZEROOUT : 0;

	if (EXT4_EXT_MAY_ZEROOUT & split_flag)
		max_zeroout = sbi->s_extent_max_zeroout_kb >>
			inode->i_sb->s_blocksize_bits;

	 
	if (max_zeroout && (ee_len <= max_zeroout)) {
		err = ext4_ext_zeroout(inode, ex);
		if (err)
			goto out;

		err = ext4_ext_get_access(handle, inode, path + depth);
		if (err)
			goto out;
		ext4_ext_mark_initialized(ex);
		ext4_ext_try_to_merge(handle, inode, path, ex);
		err = ext4_ext_dirty(handle, inode, path + path->p_depth);
		goto out;
	}

	 
	split_map.m_lblk = map->m_lblk;
	split_map.m_len = map->m_len;

	if (max_zeroout && (allocated > map->m_len)) {
		if (allocated <= max_zeroout) {
			 
			zero_ex.ee_block =
					 cpu_to_le32(map->m_lblk);
			zero_ex.ee_len = cpu_to_le16(allocated);
			ext4_ext_store_pblock(&zero_ex,
				ext4_ext_pblock(ex) + map->m_lblk - ee_block);
			err = ext4_ext_zeroout(inode, &zero_ex);
			if (err)
				goto out;
			split_map.m_lblk = map->m_lblk;
			split_map.m_len = allocated;
		} else if (map->m_lblk - ee_block + map->m_len < max_zeroout) {
			 
			if (map->m_lblk != ee_block) {
				zero_ex.ee_block = ex->ee_block;
				zero_ex.ee_len = cpu_to_le16(map->m_lblk -
							ee_block);
				ext4_ext_store_pblock(&zero_ex,
						      ext4_ext_pblock(ex));
				err = ext4_ext_zeroout(inode, &zero_ex);
				if (err)
					goto out;
			}

			split_map.m_lblk = ee_block;
			split_map.m_len = map->m_lblk - ee_block + map->m_len;
			allocated = map->m_len;
		}
	}

	allocated = ext4_split_extent(handle, inode, path,
				      &split_map, split_flag, 0);
	if (allocated < 0)
		err = allocated;

out:
	return err ? err : allocated;
}
