ext4_ext_search_right(struct inode *inode, struct ext4_ext_path *path,
			ext4_lblk_t *logical, ext4_fsblk_t *phys)
{
	struct buffer_head *bh = NULL;
	struct ext4_extent_header *eh;
	struct ext4_extent_idx *ix;
	struct ext4_extent *ex;
	ext4_fsblk_t block;
	int depth;	 
	int ee_len;

	BUG_ON(path == NULL);
	depth = path->p_depth;
	*phys = 0;

	if (depth == 0 && path->p_ext == NULL)
		return 0;

	 

	ex = path[depth].p_ext;
	ee_len = ext4_ext_get_actual_len(ex);
	if (*logical < le32_to_cpu(ex->ee_block)) {
		BUG_ON(EXT_FIRST_EXTENT(path[depth].p_hdr) != ex);
		while (--depth >= 0) {
			ix = path[depth].p_idx;
			BUG_ON(ix != EXT_FIRST_INDEX(path[depth].p_hdr));
		}
		*logical = le32_to_cpu(ex->ee_block);
		*phys = ext_pblock(ex);
		return 0;
	}

	BUG_ON(*logical < (le32_to_cpu(ex->ee_block) + ee_len));

	if (ex != EXT_LAST_EXTENT(path[depth].p_hdr)) {
		 
		ex++;
		*logical = le32_to_cpu(ex->ee_block);
		*phys = ext_pblock(ex);
		return 0;
	}

	 
	while (--depth >= 0) {
		ix = path[depth].p_idx;
		if (ix != EXT_LAST_INDEX(path[depth].p_hdr))
			goto got_index;
	}

	 
	return 0;

got_index:
	 
	ix++;
	block = idx_pblock(ix);
	while (++depth < path->p_depth) {
		bh = sb_bread(inode->i_sb, block);
		if (bh == NULL)
			return -EIO;
		eh = ext_block_hdr(bh);
		 
		if (ext4_ext_check(inode, eh, path->p_depth - depth)) {
			put_bh(bh);
			return -EIO;
		}
		ix = EXT_FIRST_INDEX(eh);
		block = idx_pblock(ix);
		put_bh(bh);
	}

	bh = sb_bread(inode->i_sb, block);
	if (bh == NULL)
		return -EIO;
	eh = ext_block_hdr(bh);
	if (ext4_ext_check(inode, eh, path->p_depth - depth)) {
		put_bh(bh);
		return -EIO;
	}
	ex = EXT_FIRST_EXTENT(eh);
	*logical = le32_to_cpu(ex->ee_block);
	*phys = ext_pblock(ex);
	put_bh(bh);
	return 0;
}
