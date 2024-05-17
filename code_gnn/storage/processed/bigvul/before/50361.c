int ocfs2_init_acl(handle_t *handle,
		   struct inode *inode,
		   struct inode *dir,
		   struct buffer_head *di_bh,
		   struct buffer_head *dir_bh,
		   struct ocfs2_alloc_context *meta_ac,
		   struct ocfs2_alloc_context *data_ac)
{
	struct ocfs2_super *osb = OCFS2_SB(inode->i_sb);
	struct posix_acl *acl = NULL;
	int ret = 0, ret2;
	umode_t mode;

	if (!S_ISLNK(inode->i_mode)) {
		if (osb->s_mount_opt & OCFS2_MOUNT_POSIX_ACL) {
			acl = ocfs2_get_acl_nolock(dir, ACL_TYPE_DEFAULT,
						   dir_bh);
			if (IS_ERR(acl))
				return PTR_ERR(acl);
		}
		if (!acl) {
			mode = inode->i_mode & ~current_umask();
			ret = ocfs2_acl_set_mode(inode, di_bh, handle, mode);
			if (ret) {
				mlog_errno(ret);
				goto cleanup;
			}
		}
	}
	if ((osb->s_mount_opt & OCFS2_MOUNT_POSIX_ACL) && acl) {
		if (S_ISDIR(inode->i_mode)) {
			ret = ocfs2_set_acl(handle, inode, di_bh,
					    ACL_TYPE_DEFAULT, acl,
					    meta_ac, data_ac);
			if (ret)
				goto cleanup;
		}
		mode = inode->i_mode;
		ret = __posix_acl_create(&acl, GFP_NOFS, &mode);
		if (ret < 0)
			return ret;

		ret2 = ocfs2_acl_set_mode(inode, di_bh, handle, mode);
		if (ret2) {
			mlog_errno(ret2);
			ret = ret2;
			goto cleanup;
		}
		if (ret > 0) {
			ret = ocfs2_set_acl(handle, inode,
					    di_bh, ACL_TYPE_ACCESS,
					    acl, meta_ac, data_ac);
		}
	}
cleanup:
	posix_acl_release(acl);
	return ret;
}
