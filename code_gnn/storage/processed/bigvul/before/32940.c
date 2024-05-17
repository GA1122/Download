static void ext3_put_super (struct super_block * sb)
{
	struct ext3_sb_info *sbi = EXT3_SB(sb);
	struct ext3_super_block *es = sbi->s_es;
	int i, err;

	dquot_disable(sb, -1, DQUOT_USAGE_ENABLED | DQUOT_LIMITS_ENABLED);
	ext3_xattr_put_super(sb);
	err = journal_destroy(sbi->s_journal);
	sbi->s_journal = NULL;
	if (err < 0)
		ext3_abort(sb, __func__, "Couldn't clean up the journal");

	if (!(sb->s_flags & MS_RDONLY)) {
		EXT3_CLEAR_INCOMPAT_FEATURE(sb, EXT3_FEATURE_INCOMPAT_RECOVER);
		es->s_state = cpu_to_le16(sbi->s_mount_state);
		BUFFER_TRACE(sbi->s_sbh, "marking dirty");
		mark_buffer_dirty(sbi->s_sbh);
		ext3_commit_super(sb, es, 1);
	}

	for (i = 0; i < sbi->s_gdb_count; i++)
		brelse(sbi->s_group_desc[i]);
	kfree(sbi->s_group_desc);
	percpu_counter_destroy(&sbi->s_freeblocks_counter);
	percpu_counter_destroy(&sbi->s_freeinodes_counter);
	percpu_counter_destroy(&sbi->s_dirs_counter);
	brelse(sbi->s_sbh);
#ifdef CONFIG_QUOTA
	for (i = 0; i < MAXQUOTAS; i++)
		kfree(sbi->s_qf_names[i]);
#endif

	 
	if (!list_empty(&sbi->s_orphan))
		dump_orphan_list(sb, sbi);
	J_ASSERT(list_empty(&sbi->s_orphan));

	invalidate_bdev(sb->s_bdev);
	if (sbi->journal_bdev && sbi->journal_bdev != sb->s_bdev) {
		 
		sync_blockdev(sbi->journal_bdev);
		invalidate_bdev(sbi->journal_bdev);
		ext3_blkdev_remove(sbi);
	}
	sb->s_fs_info = NULL;
	kfree(sbi->s_blockgroup_lock);
	kfree(sbi);
}