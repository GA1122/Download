int ext4_ext_insert_extent(handle_t *handle, struct inode *inode,
				struct ext4_ext_path *path,
				struct ext4_extent *newext, int flag)
{
	struct ext4_extent_header *eh;
	struct ext4_extent *ex, *fex;
	struct ext4_extent *nearex;  
	struct ext4_ext_path *npath = NULL;
	int depth, len, err;
	ext4_lblk_t next;
	unsigned uninitialized = 0;
	int flags = 0;

	if (unlikely(ext4_ext_get_actual_len(newext) == 0)) {
		EXT4_ERROR_INODE(inode, "ext4_ext_get_actual_len(newext) == 0");
		return -EIO;
	}
	depth = ext_depth(inode);
	ex = path[depth].p_ext;
	if (unlikely(path[depth].p_hdr == NULL)) {
		EXT4_ERROR_INODE(inode, "path[%d].p_hdr == NULL", depth);
		return -EIO;
	}

	 
	if (ex && !(flag & EXT4_GET_BLOCKS_PRE_IO)
		&& ext4_can_extents_be_merged(inode, ex, newext)) {
		ext_debug("append [%d]%d block to %u:[%d]%d (from %llu)\n",
			  ext4_ext_is_uninitialized(newext),
			  ext4_ext_get_actual_len(newext),
			  le32_to_cpu(ex->ee_block),
			  ext4_ext_is_uninitialized(ex),
			  ext4_ext_get_actual_len(ex),
			  ext4_ext_pblock(ex));
		err = ext4_ext_get_access(handle, inode, path + depth);
		if (err)
			return err;

		 
		if (ext4_ext_is_uninitialized(ex))
			uninitialized = 1;
		ex->ee_len = cpu_to_le16(ext4_ext_get_actual_len(ex)
					+ ext4_ext_get_actual_len(newext));
		if (uninitialized)
			ext4_ext_mark_uninitialized(ex);
		eh = path[depth].p_hdr;
		nearex = ex;
		goto merge;
	}

	depth = ext_depth(inode);
	eh = path[depth].p_hdr;
	if (le16_to_cpu(eh->eh_entries) < le16_to_cpu(eh->eh_max))
		goto has_space;

	 
	fex = EXT_LAST_EXTENT(eh);
	next = EXT_MAX_BLOCKS;
	if (le32_to_cpu(newext->ee_block) > le32_to_cpu(fex->ee_block))
		next = ext4_ext_next_leaf_block(path);
	if (next != EXT_MAX_BLOCKS) {
		ext_debug("next leaf block - %u\n", next);
		BUG_ON(npath != NULL);
		npath = ext4_ext_find_extent(inode, next, NULL);
		if (IS_ERR(npath))
			return PTR_ERR(npath);
		BUG_ON(npath->p_depth != path->p_depth);
		eh = npath[depth].p_hdr;
		if (le16_to_cpu(eh->eh_entries) < le16_to_cpu(eh->eh_max)) {
			ext_debug("next leaf isn't full(%d)\n",
				  le16_to_cpu(eh->eh_entries));
			path = npath;
			goto has_space;
		}
		ext_debug("next leaf has no free space(%d,%d)\n",
			  le16_to_cpu(eh->eh_entries), le16_to_cpu(eh->eh_max));
	}

	 
	if (flag & EXT4_GET_BLOCKS_PUNCH_OUT_EXT)
		flags = EXT4_MB_USE_ROOT_BLOCKS;
	err = ext4_ext_create_new_leaf(handle, inode, flags, path, newext);
	if (err)
		goto cleanup;
	depth = ext_depth(inode);
	eh = path[depth].p_hdr;

has_space:
	nearex = path[depth].p_ext;

	err = ext4_ext_get_access(handle, inode, path + depth);
	if (err)
		goto cleanup;

	if (!nearex) {
		 
		ext_debug("first extent in the leaf: %u:%llu:[%d]%d\n",
				le32_to_cpu(newext->ee_block),
				ext4_ext_pblock(newext),
				ext4_ext_is_uninitialized(newext),
				ext4_ext_get_actual_len(newext));
		nearex = EXT_FIRST_EXTENT(eh);
	} else {
		if (le32_to_cpu(newext->ee_block)
			   > le32_to_cpu(nearex->ee_block)) {
			 
			ext_debug("insert %u:%llu:[%d]%d before: "
					"nearest %p\n",
					le32_to_cpu(newext->ee_block),
					ext4_ext_pblock(newext),
					ext4_ext_is_uninitialized(newext),
					ext4_ext_get_actual_len(newext),
					nearex);
			nearex++;
		} else {
			 
			BUG_ON(newext->ee_block == nearex->ee_block);
			ext_debug("insert %u:%llu:[%d]%d after: "
					"nearest %p\n",
					le32_to_cpu(newext->ee_block),
					ext4_ext_pblock(newext),
					ext4_ext_is_uninitialized(newext),
					ext4_ext_get_actual_len(newext),
					nearex);
		}
		len = EXT_LAST_EXTENT(eh) - nearex + 1;
		if (len > 0) {
			ext_debug("insert %u:%llu:[%d]%d: "
					"move %d extents from 0x%p to 0x%p\n",
					le32_to_cpu(newext->ee_block),
					ext4_ext_pblock(newext),
					ext4_ext_is_uninitialized(newext),
					ext4_ext_get_actual_len(newext),
					len, nearex, nearex + 1);
			memmove(nearex + 1, nearex,
				len * sizeof(struct ext4_extent));
		}
	}

	le16_add_cpu(&eh->eh_entries, 1);
	path[depth].p_ext = nearex;
	nearex->ee_block = newext->ee_block;
	ext4_ext_store_pblock(nearex, ext4_ext_pblock(newext));
	nearex->ee_len = newext->ee_len;

merge:
	 
	if (!(flag & EXT4_GET_BLOCKS_PRE_IO))
		ext4_ext_try_to_merge(handle, inode, path, nearex);


	 
	err = ext4_ext_correct_indexes(handle, inode, path);
	if (err)
		goto cleanup;

	err = ext4_ext_dirty(handle, inode, path + path->p_depth);

cleanup:
	if (npath) {
		ext4_ext_drop_refs(npath);
		kfree(npath);
	}
	ext4_ext_invalidate_cache(inode);
	return err;
}