ext4_ext_search_left(struct inode *inode, struct ext4_ext_path *path,
			ext4_lblk_t *logical, ext4_fsblk_t *phys)
{
	struct ext4_extent_idx *ix;
	struct ext4_extent *ex;
	int depth, ee_len;

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
		return 0;
	}

	BUG_ON(*logical < (le32_to_cpu(ex->ee_block) + ee_len));

	*logical = le32_to_cpu(ex->ee_block) + ee_len - 1;
	*phys = ext_pblock(ex) + ee_len - 1;
	return 0;
}