void flush_sit_entries(struct f2fs_sb_info *sbi, struct cp_control *cpc)
{
	struct sit_info *sit_i = SIT_I(sbi);
	unsigned long *bitmap = sit_i->dirty_sentries_bitmap;
	struct curseg_info *curseg = CURSEG_I(sbi, CURSEG_COLD_DATA);
	struct f2fs_journal *journal = curseg->journal;
	struct sit_entry_set *ses, *tmp;
	struct list_head *head = &SM_I(sbi)->sit_entry_set;
	bool to_journal = true;
	struct seg_entry *se;

	mutex_lock(&sit_i->sentry_lock);

	if (!sit_i->dirty_sentries)
		goto out;

	 
	add_sits_in_set(sbi);

	 
	if (!__has_cursum_space(journal, sit_i->dirty_sentries, SIT_JOURNAL))
		remove_sits_in_journal(sbi);

	 
	list_for_each_entry_safe(ses, tmp, head, set_list) {
		struct page *page = NULL;
		struct f2fs_sit_block *raw_sit = NULL;
		unsigned int start_segno = ses->start_segno;
		unsigned int end = min(start_segno + SIT_ENTRY_PER_BLOCK,
						(unsigned long)MAIN_SEGS(sbi));
		unsigned int segno = start_segno;

		if (to_journal &&
			!__has_cursum_space(journal, ses->entry_cnt, SIT_JOURNAL))
			to_journal = false;

		if (to_journal) {
			down_write(&curseg->journal_rwsem);
		} else {
			page = get_next_sit_page(sbi, start_segno);
			raw_sit = page_address(page);
		}

		 
		for_each_set_bit_from(segno, bitmap, end) {
			int offset, sit_offset;

			se = get_seg_entry(sbi, segno);

			 
			if (!(cpc->reason & CP_DISCARD)) {
				cpc->trim_start = segno;
				add_discard_addrs(sbi, cpc, false);
			}

			if (to_journal) {
				offset = lookup_journal_in_cursum(journal,
							SIT_JOURNAL, segno, 1);
				f2fs_bug_on(sbi, offset < 0);
				segno_in_journal(journal, offset) =
							cpu_to_le32(segno);
				seg_info_to_raw_sit(se,
					&sit_in_journal(journal, offset));
			} else {
				sit_offset = SIT_ENTRY_OFFSET(sit_i, segno);
				seg_info_to_raw_sit(se,
						&raw_sit->entries[sit_offset]);
			}

			__clear_bit(segno, bitmap);
			sit_i->dirty_sentries--;
			ses->entry_cnt--;
		}

		if (to_journal)
			up_write(&curseg->journal_rwsem);
		else
			f2fs_put_page(page, 1);

		f2fs_bug_on(sbi, ses->entry_cnt);
		release_sit_entry_set(ses);
	}

	f2fs_bug_on(sbi, !list_empty(head));
	f2fs_bug_on(sbi, sit_i->dirty_sentries);
out:
	if (cpc->reason & CP_DISCARD) {
		__u64 trim_start = cpc->trim_start;

		for (; cpc->trim_start <= cpc->trim_end; cpc->trim_start++)
			add_discard_addrs(sbi, cpc, false);

		cpc->trim_start = trim_start;
	}
	mutex_unlock(&sit_i->sentry_lock);

	set_prefree_as_free_segments(sbi);
}