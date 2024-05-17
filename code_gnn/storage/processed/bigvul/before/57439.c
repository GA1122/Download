ext4_ext_find_extent(struct inode *inode, ext4_lblk_t block,
					struct ext4_ext_path *path)
{
	struct ext4_extent_header *eh;
	struct buffer_head *bh;
	short int depth, i, ppos = 0, alloc = 0;

	eh = ext_inode_hdr(inode);
	depth = ext_depth(inode);

	 
	if (!path) {
		path = kzalloc(sizeof(struct ext4_ext_path) * (depth + 2),
				GFP_NOFS);
		if (!path)
			return ERR_PTR(-ENOMEM);
		alloc = 1;
	}
	path[0].p_hdr = eh;
	path[0].p_bh = NULL;

	i = depth;
	 
	while (i) {
		int need_to_validate = 0;

		ext_debug("depth %d: num %d, max %d\n",
			  ppos, le16_to_cpu(eh->eh_entries), le16_to_cpu(eh->eh_max));

		ext4_ext_binsearch_idx(inode, path + ppos, block);
		path[ppos].p_block = idx_pblock(path[ppos].p_idx);
		path[ppos].p_depth = i;
		path[ppos].p_ext = NULL;

		bh = sb_getblk(inode->i_sb, path[ppos].p_block);
		if (unlikely(!bh))
			goto err;
		if (!bh_uptodate_or_lock(bh)) {
			if (bh_submit_read(bh) < 0) {
				put_bh(bh);
				goto err;
			}
			 
			need_to_validate = 1;
		}
		eh = ext_block_hdr(bh);
		ppos++;
		BUG_ON(ppos > depth);
		path[ppos].p_bh = bh;
		path[ppos].p_hdr = eh;
		i--;

		if (need_to_validate && ext4_ext_check(inode, eh, i))
			goto err;
	}

	path[ppos].p_depth = i;
	path[ppos].p_ext = NULL;
	path[ppos].p_idx = NULL;

	 
	ext4_ext_binsearch(inode, path + ppos, block);
	 
	if (path[ppos].p_ext)
		path[ppos].p_block = ext_pblock(path[ppos].p_ext);

	ext4_ext_show_path(inode, path);

	return path;

err:
	ext4_ext_drop_refs(path);
	if (alloc)
		kfree(path);
	return ERR_PTR(-EIO);
}