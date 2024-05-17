static ext4_lblk_t ext4_ext_next_leaf_block(struct ext4_ext_path *path)
{
	int depth;

	BUG_ON(path == NULL);
	depth = path->p_depth;

	 
	if (depth == 0)
		return EXT_MAX_BLOCKS;

	 
	depth--;

	while (depth >= 0) {
		if (path[depth].p_idx !=
				EXT_LAST_INDEX(path[depth].p_hdr))
			return (ext4_lblk_t)
				le32_to_cpu(path[depth].p_idx[1].ei_block);
		depth--;
	}

	return EXT_MAX_BLOCKS;
}