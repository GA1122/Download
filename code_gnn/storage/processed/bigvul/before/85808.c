int ocfs2_getattr(const struct path *path, struct kstat *stat,
		  u32 request_mask, unsigned int flags)
{
	struct inode *inode = d_inode(path->dentry);
	struct super_block *sb = path->dentry->d_sb;
	struct ocfs2_super *osb = sb->s_fs_info;
	int err;

	err = ocfs2_inode_revalidate(path->dentry);
	if (err) {
		if (err != -ENOENT)
			mlog_errno(err);
		goto bail;
	}

	generic_fillattr(inode, stat);
	 
	if (unlikely(OCFS2_I(inode)->ip_dyn_features & OCFS2_INLINE_DATA_FL))
		stat->blocks += (stat->size + 511)>>9;

	 
	stat->blksize = osb->s_clustersize;

bail:
	return err;
}
