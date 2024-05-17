 static int ext4_ext_split(handle_t *handle, struct inode *inode,
 			  unsigned int flags,
 			  struct ext4_ext_path *path,
 			  struct ext4_extent *newext, int at)
 {
 	struct buffer_head *bh = NULL;
 	int depth = ext_depth(inode);
 	struct ext4_extent_header *neh;
 	struct ext4_extent_idx *fidx;
 	int i = at, k, m, a;
 	ext4_fsblk_t newblock, oldblock;
  	__le32 border;
  	ext4_fsblk_t *ablocks = NULL;  
  	int err = 0;
// 	size_t ext_size = 0;
  
  	 
  	 
 
 	 
 	if (unlikely(path[depth].p_ext > EXT_MAX_EXTENT(path[depth].p_hdr))) {
 		EXT4_ERROR_INODE(inode, "p_ext > EXT_MAX_EXTENT!");
 		return -EFSCORRUPTED;
 	}
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
 
 	 
 
 	 
 	ablocks = kcalloc(depth, sizeof(ext4_fsblk_t), GFP_NOFS);
 	if (!ablocks)
 		return -ENOMEM;
 
 	 
 	ext_debug("allocate %d blocks for indexes/leaf\n", depth - at);
 	for (a = 0; a < depth - at; a++) {
 		newblock = ext4_ext_new_meta_block(handle, inode, path,
 						   newext, &err, flags);
 		if (newblock == 0)
 			goto cleanup;
 		ablocks[a] = newblock;
 	}
 
 	 
 	newblock = ablocks[--a];
 	if (unlikely(newblock == 0)) {
 		EXT4_ERROR_INODE(inode, "newblock == 0!");
 		err = -EFSCORRUPTED;
 		goto cleanup;
 	}
 	bh = sb_getblk_gfp(inode->i_sb, newblock, __GFP_MOVABLE | GFP_NOFS);
 	if (unlikely(!bh)) {
 		err = -ENOMEM;
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
 
 	 
 	if (unlikely(path[depth].p_hdr->eh_entries !=
 		     path[depth].p_hdr->eh_max)) {
 		EXT4_ERROR_INODE(inode, "eh_entries %d != eh_max %d!",
 				 path[depth].p_hdr->eh_entries,
 				 path[depth].p_hdr->eh_max);
 		err = -EFSCORRUPTED;
 		goto cleanup;
 	}
 	 
 	m = EXT_MAX_EXTENT(path[depth].p_hdr) - path[depth].p_ext++;
 	ext4_ext_show_move(inode, path, newblock, depth);
 	if (m) {
 		struct ext4_extent *ex;
 		ex = EXT_FIRST_EXTENT(neh);
 		memmove(ex, path[depth].p_ext, sizeof(struct ext4_extent) * m);
  		le16_add_cpu(&neh->eh_entries, m);
  	}
  
// 	 
// 	ext_size = sizeof(struct ext4_extent_header) +
// 		sizeof(struct ext4_extent) * le16_to_cpu(neh->eh_entries);
// 	memset(bh->b_data + ext_size, 0, inode->i_sb->s_blocksize - ext_size);
  	ext4_extent_block_csum_set(inode, neh);
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
 	if (unlikely(k < 0)) {
 		EXT4_ERROR_INODE(inode, "k %d < 0!", k);
 		err = -EFSCORRUPTED;
 		goto cleanup;
 	}
 	if (k)
 		ext_debug("create %d intermediate indices\n", k);
 	 
 	 
 	i = depth - 1;
 	while (k--) {
 		oldblock = newblock;
 		newblock = ablocks[--a];
 		bh = sb_getblk(inode->i_sb, newblock);
 		if (unlikely(!bh)) {
 			err = -ENOMEM;
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
 
 		 
 		if (unlikely(EXT_MAX_INDEX(path[i].p_hdr) !=
 					EXT_LAST_INDEX(path[i].p_hdr))) {
 			EXT4_ERROR_INODE(inode,
 					 "EXT_MAX_INDEX != EXT_LAST_INDEX ee_block %d!",
 					 le32_to_cpu(path[i].p_ext->ee_block));
 			err = -EFSCORRUPTED;
 			goto cleanup;
 		}
 		 
 		m = EXT_MAX_INDEX(path[i].p_hdr) - path[i].p_idx++;
 		ext_debug("cur 0x%p, last 0x%p\n", path[i].p_idx,
 				EXT_MAX_INDEX(path[i].p_hdr));
 		ext4_ext_show_move(inode, path, newblock, i);
 		if (m) {
 			memmove(++fidx, path[i].p_idx,
  				sizeof(struct ext4_extent_idx) * m);
  			le16_add_cpu(&neh->eh_entries, m);
  		}
// 		 
// 		ext_size = sizeof(struct ext4_extent_header) +
// 		   (sizeof(struct ext4_extent) * le16_to_cpu(neh->eh_entries));
// 		memset(bh->b_data + ext_size, 0,
// 			inode->i_sb->s_blocksize - ext_size);
  		ext4_extent_block_csum_set(inode, neh);
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
 			ext4_free_blocks(handle, inode, NULL, ablocks[i], 1,
 					 EXT4_FREE_BLOCKS_METADATA);
 		}
 	}
 	kfree(ablocks);
 
 	return err;
 }