static int ext4_sync_fs(struct super_block *sb, int wait)
{
	int ret = 0;
	tid_t target;
	bool needs_barrier = false;
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	trace_ext4_sync_fs(sb, wait);
	flush_workqueue(sbi->rsv_conversion_wq);
	 
	dquot_writeback_dquots(sb, -1);
	 
	if (sbi->s_journal) {
		target = jbd2_get_latest_transaction(sbi->s_journal);
		if (wait && sbi->s_journal->j_flags & JBD2_BARRIER &&
		    !jbd2_trans_will_send_data_barrier(sbi->s_journal, target))
			needs_barrier = true;

		if (jbd2_journal_start_commit(sbi->s_journal, &target)) {
			if (wait)
				ret = jbd2_log_wait_commit(sbi->s_journal,
							   target);
		}
	} else if (wait && test_opt(sb, BARRIER))
		needs_barrier = true;
	if (needs_barrier) {
		int err;
		err = blkdev_issue_flush(sb->s_bdev, GFP_KERNEL, NULL);
		if (!ret)
			ret = err;
	}

	return ret;
}