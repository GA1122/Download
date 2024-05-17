static int ext4_remove_blocks(handle_t *handle, struct inode *inode,
			      struct ext4_extent *ex,
			      long long *partial_cluster,
			      ext4_lblk_t from, ext4_lblk_t to)
{
	struct ext4_sb_info *sbi = EXT4_SB(inode->i_sb);
	unsigned short ee_len = ext4_ext_get_actual_len(ex);
	ext4_fsblk_t pblk;
	int flags = get_default_free_blocks_flags(inode);

	 
	flags |= EXT4_FREE_BLOCKS_NOFREE_FIRST_CLUSTER;

	trace_ext4_remove_blocks(inode, ex, from, to, *partial_cluster);
	 
	pblk = ext4_ext_pblock(ex) + ee_len - 1;
	if (*partial_cluster > 0 &&
	    *partial_cluster != (long long) EXT4_B2C(sbi, pblk)) {
		ext4_free_blocks(handle, inode, NULL,
				 EXT4_C2B(sbi, *partial_cluster),
				 sbi->s_cluster_ratio, flags);
		*partial_cluster = 0;
	}

#ifdef EXTENTS_STATS
	{
		struct ext4_sb_info *sbi = EXT4_SB(inode->i_sb);
		spin_lock(&sbi->s_ext_stats_lock);
		sbi->s_ext_blocks += ee_len;
		sbi->s_ext_extents++;
		if (ee_len < sbi->s_ext_min)
			sbi->s_ext_min = ee_len;
		if (ee_len > sbi->s_ext_max)
			sbi->s_ext_max = ee_len;
		if (ext_depth(inode) > sbi->s_depth_max)
			sbi->s_depth_max = ext_depth(inode);
		spin_unlock(&sbi->s_ext_stats_lock);
	}
#endif
	if (from >= le32_to_cpu(ex->ee_block)
	    && to == le32_to_cpu(ex->ee_block) + ee_len - 1) {
		 
		ext4_lblk_t num;
		long long first_cluster;

		num = le32_to_cpu(ex->ee_block) + ee_len - from;
		pblk = ext4_ext_pblock(ex) + ee_len - num;
		 
		if (*partial_cluster < 0 &&
		    *partial_cluster == -(long long) EXT4_B2C(sbi, pblk+num-1))
			flags |= EXT4_FREE_BLOCKS_NOFREE_LAST_CLUSTER;

		ext_debug("free last %u blocks starting %llu partial %lld\n",
			  num, pblk, *partial_cluster);
		ext4_free_blocks(handle, inode, NULL, pblk, num, flags);
		 
		if (EXT4_PBLK_COFF(sbi, pblk) && ee_len == num) {
			first_cluster = (long long) EXT4_B2C(sbi, pblk);
			if (first_cluster != -*partial_cluster)
				*partial_cluster = first_cluster;
		} else {
			*partial_cluster = 0;
		}
	} else
		ext4_error(sbi->s_sb, "strange request: removal(2) "
			   "%u-%u from %u:%u\n",
			   from, to, le32_to_cpu(ex->ee_block), ee_len);
	return 0;
}
