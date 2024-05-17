 int build_segment_manager(struct f2fs_sb_info *sbi)
 {
 	struct f2fs_super_block *raw_super = F2FS_RAW_SUPER(sbi);
 	struct f2fs_checkpoint *ckpt = F2FS_CKPT(sbi);
 	struct f2fs_sm_info *sm_info;
 	int err;
 
 	sm_info = kzalloc(sizeof(struct f2fs_sm_info), GFP_KERNEL);
 	if (!sm_info)
 		return -ENOMEM;
 
 	 
 	sbi->sm_info = sm_info;
 	sm_info->seg0_blkaddr = le32_to_cpu(raw_super->segment0_blkaddr);
 	sm_info->main_blkaddr = le32_to_cpu(raw_super->main_blkaddr);
 	sm_info->segment_count = le32_to_cpu(raw_super->segment_count);
 	sm_info->reserved_segments = le32_to_cpu(ckpt->rsvd_segment_count);
 	sm_info->ovp_segments = le32_to_cpu(ckpt->overprov_segment_count);
 	sm_info->main_segments = le32_to_cpu(raw_super->segment_count_main);
 	sm_info->ssa_blkaddr = le32_to_cpu(raw_super->ssa_blkaddr);
 	sm_info->rec_prefree_segments = sm_info->main_segments *
 					DEF_RECLAIM_PREFREE_SEGMENTS / 100;
 	if (sm_info->rec_prefree_segments > DEF_MAX_RECLAIM_PREFREE_SEGMENTS)
 		sm_info->rec_prefree_segments = DEF_MAX_RECLAIM_PREFREE_SEGMENTS;
 
 	if (!test_opt(sbi, LFS))
 		sm_info->ipu_policy = 1 << F2FS_IPU_FSYNC;
 	sm_info->min_ipu_util = DEF_MIN_IPU_UTIL;
 	sm_info->min_fsync_blocks = DEF_MIN_FSYNC_BLOCKS;
 	sm_info->min_hot_blocks = DEF_MIN_HOT_BLOCKS;
 
 	sm_info->trim_sections = DEF_BATCHED_TRIM_SECTIONS;
  
  	INIT_LIST_HEAD(&sm_info->sit_entry_set);
  
	if (test_opt(sbi, FLUSH_MERGE) && !f2fs_readonly(sbi->sb)) {
// 	if (!f2fs_readonly(sbi->sb)) {
  		err = create_flush_cmd_control(sbi);
  		if (err)
  			return err;
 	}
 
 	err = create_discard_cmd_control(sbi);
 	if (err)
 		return err;
 
 	err = build_sit_info(sbi);
 	if (err)
 		return err;
 	err = build_free_segmap(sbi);
 	if (err)
 		return err;
 	err = build_curseg(sbi);
 	if (err)
 		return err;
 
 	 
 	build_sit_entries(sbi);
 
 	init_free_segmap(sbi);
 	err = build_dirty_segmap(sbi);
 	if (err)
 		return err;
 
 	init_min_max_mtime(sbi);
 	return 0;
 }