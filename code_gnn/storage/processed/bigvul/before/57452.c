static int ext4_ext_split(handle_t *handle, struct inode *inode,
				struct ext4_ext_path *path,
				struct ext4_extent *newext, int at)
{
	struct buffer_head *bh = NULL;
	int depth = ext_depth(inode);
	struct ext4_extent_header *neh;
	struct ext4_extent_idx *fidx;
	struct ext4_extent *ex;
	int i = at, k, m, a;
	ext4_fsblk_t newblock, oldblock;
	__le32 border;
	ext4_fsblk_t *ablocks = NULL;  
	int err = 0;

	 
	 

	 
	BUG_ON(path[depth].p_ext > EXT_MAX_EXTENT(path[depth].p_hdr));
	if (path[depth].p_ext != EXT_MAX_EXTENT(path[depth].p_hdr)) {
		border = path[depth].p_ext[1].ee_block;
		ext_debug("leaf will be split."
				" next leaf starts at %d\n",
				  le32_to_cpu(border));
	} else {
		border = newext->ee_block;
		ext_debug("leaf will be added."
				" next leaf starts at %d\n",
				le32_to_cpu(border));
	}

	 

	 
	ablocks = kzalloc(sizeof(ext4_fsblk_t) * depth, GFP_NOFS);
	if (!ablocks)
		return -ENOMEM;

	 
	ext_debug("allocate %d blocks for indexes/leaf\n", depth - at);
	for (a = 0; a < depth - at; a++) {
		newblock = ext4_ext_new_meta_block(handle, inode, path,
						   newext, &err);
		if (newblock == 0)
			goto cleanup;
		ablocks[a] = newblock;
	}

	 
	newblock = ablocks[--a];
	BUG_ON(newblock == 0);
	bh = sb_getblk(inode->i_sb, newblock);
	if (!bh) {
		err = -EIO;
		goto cleanup;
	}
	lock_buffer(bh);

	err = ext4_journal_get_create_access(handle, bh);
	if (err)
		goto cleanup;

	neh = ext_block_hdr(bh);
	neh->eh_entries = 0;
	neh->eh_max = cpu_to_le16(ext4_ext_space_block(inode, 0));
	neh->eh_magic = EXT4_EXT_MAGIC;
	neh->eh_depth = 0;
	ex = EXT_FIRST_EXTENT(neh);

	 
	BUG_ON(path[depth].p_hdr->eh_entries != path[depth].p_hdr->eh_max);
	 
	 
	m = 0;
	path[depth].p_ext++;
	while (path[depth].p_ext <=
			EXT_MAX_EXTENT(path[depth].p_hdr)) {
		ext_debug("move %d:%llu:[%d]%d in new leaf %llu\n",
				le32_to_cpu(path[depth].p_ext->ee_block),
				ext_pblock(path[depth].p_ext),
				ext4_ext_is_uninitialized(path[depth].p_ext),
				ext4_ext_get_actual_len(path[depth].p_ext),
				newblock);
		 
		path[depth].p_ext++;
		m++;
	}
	if (m) {
		memmove(ex, path[depth].p_ext-m, sizeof(struct ext4_extent)*m);
		le16_add_cpu(&neh->eh_entries, m);
	}

	set_buffer_uptodate(bh);
	unlock_buffer(bh);

	err = ext4_handle_dirty_metadata(handle, inode, bh);
	if (err)
		goto cleanup;
	brelse(bh);
	bh = NULL;

	 
	if (m) {
		err = ext4_ext_get_access(handle, inode, path + depth);
		if (err)
			goto cleanup;
		le16_add_cpu(&path[depth].p_hdr->eh_entries, -m);
		err = ext4_ext_dirty(handle, inode, path + depth);
		if (err)
			goto cleanup;

	}

	 
	k = depth - at - 1;
	BUG_ON(k < 0);
	if (k)
		ext_debug("create %d intermediate indices\n", k);
	 
	 
	i = depth - 1;
	while (k--) {
		oldblock = newblock;
		newblock = ablocks[--a];
		bh = sb_getblk(inode->i_sb, newblock);
		if (!bh) {
			err = -EIO;
			goto cleanup;
		}
		lock_buffer(bh);

		err = ext4_journal_get_create_access(handle, bh);
		if (err)
			goto cleanup;

		neh = ext_block_hdr(bh);
		neh->eh_entries = cpu_to_le16(1);
		neh->eh_magic = EXT4_EXT_MAGIC;
		neh->eh_max = cpu_to_le16(ext4_ext_space_block_idx(inode, 0));
		neh->eh_depth = cpu_to_le16(depth - i);
		fidx = EXT_FIRST_INDEX(neh);
		fidx->ei_block = border;
		ext4_idx_store_pblock(fidx, oldblock);

		ext_debug("int.index at %d (block %llu): %u -> %llu\n",
				i, newblock, le32_to_cpu(border), oldblock);
		 
		m = 0;
		path[i].p_idx++;

		ext_debug("cur 0x%p, last 0x%p\n", path[i].p_idx,
				EXT_MAX_INDEX(path[i].p_hdr));
		BUG_ON(EXT_MAX_INDEX(path[i].p_hdr) !=
				EXT_LAST_INDEX(path[i].p_hdr));
		while (path[i].p_idx <= EXT_MAX_INDEX(path[i].p_hdr)) {
			ext_debug("%d: move %d:%llu in new index %llu\n", i,
					le32_to_cpu(path[i].p_idx->ei_block),
					idx_pblock(path[i].p_idx),
					newblock);
			 
			path[i].p_idx++;
			m++;
		}
		if (m) {
			memmove(++fidx, path[i].p_idx - m,
				sizeof(struct ext4_extent_idx) * m);
			le16_add_cpu(&neh->eh_entries, m);
		}
		set_buffer_uptodate(bh);
		unlock_buffer(bh);

		err = ext4_handle_dirty_metadata(handle, inode, bh);
		if (err)
			goto cleanup;
		brelse(bh);
		bh = NULL;

		 
		if (m) {
			err = ext4_ext_get_access(handle, inode, path + i);
			if (err)
				goto cleanup;
			le16_add_cpu(&path[i].p_hdr->eh_entries, -m);
			err = ext4_ext_dirty(handle, inode, path + i);
			if (err)
				goto cleanup;
		}

		i--;
	}

	 
	err = ext4_ext_insert_index(handle, inode, path + at,
				    le32_to_cpu(border), newblock);

cleanup:
	if (bh) {
		if (buffer_locked(bh))
			unlock_buffer(bh);
		brelse(bh);
	}

	if (err) {
		 
		for (i = 0; i < depth; i++) {
			if (!ablocks[i])
				continue;
			ext4_free_blocks(handle, inode, 0, ablocks[i], 1,
					 EXT4_FREE_BLOCKS_METADATA);
		}
	}
	kfree(ablocks);

	return err;
}