static void ext4_put_super(struct super_block *sb)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	int i, err;

	flush_workqueue(sbi->dio_unwritten_wq);
	destroy_workqueue(sbi->dio_unwritten_wq);

	lock_super(sb);
	lock_kernel();
	if (sb->s_dirt)
		ext4_commit_super(sb, 1);

	if (sbi->s_journal) {
		err = jbd2_journal_destroy(sbi->s_journal);
		sbi->s_journal = NULL;
		if (err < 0)
			ext4_abort(sb, __func__,
				   "Couldn't clean up the journal");
	}

	ext4_release_system_zone(sb);
	ext4_mb_release(sb);
	ext4_ext_release(sb);
	ext4_xattr_put_super(sb);

	if (!(sb->s_flags & MS_RDONLY)) {
		EXT4_CLEAR_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_RECOVER);
		es->s_state = cpu_to_le16(sbi->s_mount_state);
		ext4_commit_super(sb, 1);
	}
	if (sbi->s_proc) {
		remove_proc_entry(sb->s_id, ext4_proc_root);
	}
	kobject_del(&sbi->s_kobj);

	for (i = 0; i < sbi->s_gdb_count; i++)
		brelse(sbi->s_group_desc[i]);
	kfree(sbi->s_group_desc);
	if (is_vmalloc_addr(sbi->s_flex_groups))
		vfree(sbi->s_flex_groups);
	else
		kfree(sbi->s_flex_groups);
	percpu_counter_destroy(&sbi->s_freeblocks_counter);
	percpu_counter_destroy(&sbi->s_freeinodes_counter);
	percpu_counter_destroy(&sbi->s_dirs_counter);
	percpu_counter_destroy(&sbi->s_dirtyblocks_counter);
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
		ext4_blkdev_remove(sbi);
	}
	sb->s_fs_info = NULL;
	 
	unlock_kernel();
	unlock_super(sb);
	kobject_put(&sbi->s_kobj);
	wait_for_completion(&sbi->s_kobj_unregister);
	kfree(sbi->s_blockgroup_lock);
	kfree(sbi);
}