int ocfs2_permission(struct inode *inode, int mask)
{
	int ret, had_lock;
	struct ocfs2_lock_holder oh;

	if (mask & MAY_NOT_BLOCK)
		return -ECHILD;

	had_lock = ocfs2_inode_lock_tracker(inode, NULL, 0, &oh);
	if (had_lock < 0) {
		ret = had_lock;
		goto out;
	} else if (had_lock) {
		 
		mlog(ML_ERROR, "Another case of recursive locking:\n");
		dump_stack();
	}

	ret = generic_permission(inode, mask);

	ocfs2_inode_unlock_tracker(inode, 0, &oh, had_lock);
out:
	return ret;
}