int ocfs2_iop_set_acl(struct inode *inode, struct posix_acl *acl, int type)
{
	struct buffer_head *bh = NULL;
	int status = 0;

	status = ocfs2_inode_lock(inode, &bh, 1);
	if (status < 0) {
		if (status != -ENOENT)
			mlog_errno(status);
		return status;
	}
	status = ocfs2_set_acl(NULL, inode, bh, type, acl, NULL, NULL);
	ocfs2_inode_unlock(inode, 1);
	brelse(bh);
	return status;
}
