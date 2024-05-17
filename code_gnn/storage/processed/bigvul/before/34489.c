static void do_async_commit(struct work_struct *work)
{
	struct btrfs_async_commit *ac =
		container_of(work, struct btrfs_async_commit, work.work);

	 
	if (ac->newtrans->type < TRANS_JOIN_NOLOCK)
		rwsem_acquire_read(
		     &ac->root->fs_info->sb->s_writers.lock_map[SB_FREEZE_FS-1],
		     0, 1, _THIS_IP_);

	current->journal_info = ac->newtrans;

	btrfs_commit_transaction(ac->newtrans, ac->root);
	kfree(ac);
}
