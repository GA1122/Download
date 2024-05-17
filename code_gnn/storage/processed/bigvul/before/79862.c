static void update_ovl_inode_times(struct dentry *dentry, struct inode *inode,
			       bool rcu)
{
	struct dentry *upperdentry;

	 
	if (rcu || likely(!(dentry->d_flags & DCACHE_OP_REAL)))
		return;

	upperdentry = d_real(dentry, NULL, 0, D_REAL_UPPER);

	 
	if (upperdentry) {
		struct inode *realinode = d_inode(upperdentry);

		if ((!timespec64_equal(&inode->i_mtime, &realinode->i_mtime) ||
		     !timespec64_equal(&inode->i_ctime, &realinode->i_ctime))) {
			inode->i_mtime = realinode->i_mtime;
			inode->i_ctime = realinode->i_ctime;
		}
	}
}
