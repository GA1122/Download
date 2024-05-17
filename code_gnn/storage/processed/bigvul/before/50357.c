int ocfs2_acl_chmod(struct inode *inode, struct buffer_head *bh)
{
	struct ocfs2_super *osb = OCFS2_SB(inode->i_sb);
	struct posix_acl *acl;
	int ret;

	if (S_ISLNK(inode->i_mode))
		return -EOPNOTSUPP;

	if (!(osb->s_mount_opt & OCFS2_MOUNT_POSIX_ACL))
		return 0;

	acl = ocfs2_get_acl_nolock(inode, ACL_TYPE_ACCESS, bh);
	if (IS_ERR(acl) || !acl)
		return PTR_ERR(acl);
	ret = __posix_acl_chmod(&acl, GFP_KERNEL, inode->i_mode);
	if (ret)
		return ret;
	ret = ocfs2_set_acl(NULL, inode, NULL, ACL_TYPE_ACCESS,
			    acl, NULL, NULL);
	posix_acl_release(acl);
	return ret;
}
