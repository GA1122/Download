static void ext4_set_resv_clusters(struct super_block *sb)
{
	ext4_fsblk_t resv_clusters;
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	 
	if (!ext4_has_feature_extents(sb))
		return;
	 
	resv_clusters = (ext4_blocks_count(sbi->s_es) >>
			 sbi->s_cluster_bits);

	do_div(resv_clusters, 50);
	resv_clusters = min_t(ext4_fsblk_t, resv_clusters, 4096);

	atomic64_set(&sbi->s_resv_clusters, resv_clusters);
}
