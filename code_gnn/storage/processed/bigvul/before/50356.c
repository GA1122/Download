int jfs_set_acl(struct inode *inode, struct posix_acl *acl, int type)
{
	int rc;
	tid_t tid;

	tid = txBegin(inode->i_sb, 0);
	mutex_lock(&JFS_IP(inode)->commit_mutex);
	rc = __jfs_set_acl(tid, inode, type, acl);
	if (!rc)
		rc = txCommit(tid, 1, &inode, 0);
	txEnd(tid);
	mutex_unlock(&JFS_IP(inode)->commit_mutex);
	return rc;
}
