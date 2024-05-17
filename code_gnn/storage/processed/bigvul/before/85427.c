static void update_sit_entry(struct f2fs_sb_info *sbi, block_t blkaddr, int del)
{
	struct seg_entry *se;
	unsigned int segno, offset;
	long int new_vblocks;

	segno = GET_SEGNO(sbi, blkaddr);

	se = get_seg_entry(sbi, segno);
	new_vblocks = se->valid_blocks + del;
	offset = GET_BLKOFF_FROM_SEG0(sbi, blkaddr);

	f2fs_bug_on(sbi, (new_vblocks >> (sizeof(unsigned short) << 3) ||
				(new_vblocks > sbi->blocks_per_seg)));

	se->valid_blocks = new_vblocks;
	se->mtime = get_mtime(sbi);
	SIT_I(sbi)->max_mtime = se->mtime;

	 
	if (del > 0) {
		if (f2fs_test_and_set_bit(offset, se->cur_valid_map)) {
#ifdef CONFIG_F2FS_CHECK_FS
			if (f2fs_test_and_set_bit(offset,
						se->cur_valid_map_mir))
				f2fs_bug_on(sbi, 1);
			else
				WARN_ON(1);
#else
			f2fs_bug_on(sbi, 1);
#endif
		}
		if (f2fs_discard_en(sbi) &&
			!f2fs_test_and_set_bit(offset, se->discard_map))
			sbi->discard_blks--;

		 
		if (se->type == CURSEG_WARM_NODE) {
			if (!f2fs_test_and_set_bit(offset, se->ckpt_valid_map))
				se->ckpt_valid_blocks++;
		}
	} else {
		if (!f2fs_test_and_clear_bit(offset, se->cur_valid_map)) {
#ifdef CONFIG_F2FS_CHECK_FS
			if (!f2fs_test_and_clear_bit(offset,
						se->cur_valid_map_mir))
				f2fs_bug_on(sbi, 1);
			else
				WARN_ON(1);
#else
			f2fs_bug_on(sbi, 1);
#endif
		}
		if (f2fs_discard_en(sbi) &&
			f2fs_test_and_clear_bit(offset, se->discard_map))
			sbi->discard_blks++;
	}
	if (!f2fs_test_bit(offset, se->ckpt_valid_map))
		se->ckpt_valid_blocks += del;

	__mark_sit_entry_dirty(sbi, segno);

	 
	SIT_I(sbi)->written_valid_blocks += del;

	if (sbi->segs_per_sec > 1)
		get_sec_entry(sbi, segno)->valid_blocks += del;
}