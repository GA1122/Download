static int ext4_remove_blocks(handle_t *handle, struct inode *inode,
				struct ext4_extent *ex,
				ext4_lblk_t from, ext4_lblk_t to)
{
	unsigned short ee_len =  ext4_ext_get_actual_len(ex);
	int flags = EXT4_FREE_BLOCKS_FORGET;

	if (S_ISDIR(inode->i_mode) || S_ISLNK(inode->i_mode))
		flags |= EXT4_FREE_BLOCKS_METADATA;
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
		ext4_fsblk_t start;

		num = le32_to_cpu(ex->ee_block) + ee_len - from;
		start = ext_pblock(ex) + ee_len - num;
		ext_debug("free last %u blocks starting %llu\n", num, start);
		ext4_free_blocks(handle, inode, 0, start, num, flags);
	} else if (from == le32_to_cpu(ex->ee_block)
		   && to <= le32_to_cpu(ex->ee_block) + ee_len - 1) {
		printk(KERN_INFO "strange request: removal %u-%u from %u:%u\n",
			from, to, le32_to_cpu(ex->ee_block), ee_len);
	} else {
		printk(KERN_INFO "strange request: removal(2) "
				"%u-%u from %u:%u\n",
				from, to, le32_to_cpu(ex->ee_block), ee_len);
	}
	return 0;
}