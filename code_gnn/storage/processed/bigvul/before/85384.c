void f2fs_balance_fs(struct f2fs_sb_info *sbi, bool need)
{
#ifdef CONFIG_F2FS_FAULT_INJECTION
	if (time_to_inject(sbi, FAULT_CHECKPOINT)) {
		f2fs_show_injection_info(FAULT_CHECKPOINT);
		f2fs_stop_checkpoint(sbi, false);
	}
#endif

	 
	if (need && excess_cached_nats(sbi))
		f2fs_balance_fs_bg(sbi);

	 
	if (has_not_enough_free_secs(sbi, 0, 0)) {
		mutex_lock(&sbi->gc_mutex);
		f2fs_gc(sbi, false, false, NULL_SEGNO);
	}
}