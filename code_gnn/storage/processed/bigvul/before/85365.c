static void build_sit_entries(struct f2fs_sb_info *sbi)
{
	struct sit_info *sit_i = SIT_I(sbi);
	struct curseg_info *curseg = CURSEG_I(sbi, CURSEG_COLD_DATA);
	struct f2fs_journal *journal = curseg->journal;
	struct seg_entry *se;
	struct f2fs_sit_entry sit;
	int sit_blk_cnt = SIT_BLK_CNT(sbi);
	unsigned int i, start, end;
	unsigned int readed, start_blk = 0;

	do {
		readed = ra_meta_pages(sbi, start_blk, BIO_MAX_PAGES,
							META_SIT, true);

		start = start_blk * sit_i->sents_per_block;
		end = (start_blk + readed) * sit_i->sents_per_block;

		for (; start < end && start < MAIN_SEGS(sbi); start++) {
			struct f2fs_sit_block *sit_blk;
			struct page *page;

			se = &sit_i->sentries[start];
			page = get_current_sit_page(sbi, start);
			sit_blk = (struct f2fs_sit_block *)page_address(page);
			sit = sit_blk->entries[SIT_ENTRY_OFFSET(sit_i, start)];
			f2fs_put_page(page, 1);

			check_block_count(sbi, start, &sit);
			seg_info_from_raw_sit(se, &sit);

			 
			if (f2fs_discard_en(sbi)) {
				if (is_set_ckpt_flags(sbi, CP_TRIMMED_FLAG)) {
					memset(se->discard_map, 0xff,
						SIT_VBLOCK_MAP_SIZE);
				} else {
					memcpy(se->discard_map,
						se->cur_valid_map,
						SIT_VBLOCK_MAP_SIZE);
					sbi->discard_blks +=
						sbi->blocks_per_seg -
						se->valid_blocks;
				}
			}

			if (sbi->segs_per_sec > 1)
				get_sec_entry(sbi, start)->valid_blocks +=
							se->valid_blocks;
		}
		start_blk += readed;
	} while (start_blk < sit_blk_cnt);

	down_read(&curseg->journal_rwsem);
	for (i = 0; i < sits_in_cursum(journal); i++) {
		unsigned int old_valid_blocks;

		start = le32_to_cpu(segno_in_journal(journal, i));
		se = &sit_i->sentries[start];
		sit = sit_in_journal(journal, i);

		old_valid_blocks = se->valid_blocks;

		check_block_count(sbi, start, &sit);
		seg_info_from_raw_sit(se, &sit);

		if (f2fs_discard_en(sbi)) {
			if (is_set_ckpt_flags(sbi, CP_TRIMMED_FLAG)) {
				memset(se->discard_map, 0xff,
							SIT_VBLOCK_MAP_SIZE);
			} else {
				memcpy(se->discard_map, se->cur_valid_map,
							SIT_VBLOCK_MAP_SIZE);
				sbi->discard_blks += old_valid_blocks -
							se->valid_blocks;
			}
		}

		if (sbi->segs_per_sec > 1)
			get_sec_entry(sbi, start)->valid_blocks +=
				se->valid_blocks - old_valid_blocks;
	}
	up_read(&curseg->journal_rwsem);
}