struct posix_acl *gfs2_get_acl(struct inode *inode, int type)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	bool need_unlock = false;
	struct posix_acl *acl;

	if (!gfs2_glock_is_locked_by_me(ip->i_gl)) {
		int ret = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED,
					     LM_FLAG_ANY, &gh);
		if (ret)
			return ERR_PTR(ret);
		need_unlock = true;
	}
	acl = __gfs2_get_acl(inode, type);
	if (need_unlock)
		gfs2_glock_dq_uninit(&gh);
	return acl;
}
