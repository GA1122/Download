static int restore_curseg_summaries(struct f2fs_sb_info *sbi)
{
	int type = CURSEG_HOT_DATA;
	int err;

	if (is_set_ckpt_flags(sbi, CP_COMPACT_SUM_FLAG)) {
		int npages = npages_for_summary_flush(sbi, true);

		if (npages >= 2)
			ra_meta_pages(sbi, start_sum_block(sbi), npages,
							META_CP, true);

		 
		if (read_compacted_summaries(sbi))
			return -EINVAL;
		type = CURSEG_HOT_NODE;
	}

	if (__exist_node_summaries(sbi))
		ra_meta_pages(sbi, sum_blk_addr(sbi, NR_CURSEG_TYPE, type),
					NR_CURSEG_TYPE - type, META_CP, true);

	for (; type <= CURSEG_COLD_NODE; type++) {
		err = read_normal_summaries(sbi, type);
		if (err)
			return err;
	}

	return 0;
}