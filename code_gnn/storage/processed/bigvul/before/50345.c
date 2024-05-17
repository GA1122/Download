int gfs2_set_acl(struct inode *inode, struct posix_acl *acl, int type)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	bool need_unlock = false;
	int ret;

	ret = gfs2_rsqa_alloc(ip);
	if (ret)
		return ret;

	if (!gfs2_glock_is_locked_by_me(ip->i_gl)) {
		ret = gfs2_glock_nq_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, &gh);
		if (ret)
			return ret;
		need_unlock = true;
	}
	ret = __gfs2_set_acl(inode, acl, type);
	if (need_unlock)
		gfs2_glock_dq_uninit(&gh);
	return ret;
}
