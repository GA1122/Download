int ext4_meta_trans_blocks(struct inode *inode, int nrblocks, int chunk)
{
	ext4_group_t groups, ngroups = ext4_get_groups_count(inode->i_sb);
	int gdpblocks;
	int idxblocks;
	int ret = 0;

	 
	idxblocks = ext4_index_trans_blocks(inode, nrblocks, chunk);

	ret = idxblocks;

	 
	groups = idxblocks;
	if (chunk)
		groups += 1;
	else
		groups += nrblocks;

	gdpblocks = groups;
	if (groups > ngroups)
		groups = ngroups;
	if (groups > EXT4_SB(inode->i_sb)->s_gdb_count)
		gdpblocks = EXT4_SB(inode->i_sb)->s_gdb_count;

	 
	ret += groups + gdpblocks;

	 
	ret += EXT4_META_TRANS_BLOCKS(inode->i_sb);

	return ret;
}
