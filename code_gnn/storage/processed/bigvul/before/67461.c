static inline int step_into(struct nameidata *nd, struct path *path,
			    int flags, struct inode *inode, unsigned seq)
{
	if (!(flags & WALK_MORE) && nd->depth)
		put_link(nd);
	if (likely(!d_is_symlink(path->dentry)) ||
	   !(flags & WALK_FOLLOW || nd->flags & LOOKUP_FOLLOW)) {
		 
		path_to_nameidata(path, nd);
		nd->inode = inode;
		nd->seq = seq;
		return 0;
	}
	 
	if (nd->flags & LOOKUP_RCU) {
		if (read_seqcount_retry(&path->dentry->d_seq, seq))
			return -ECHILD;
	}
	return pick_link(nd, path, inode, seq);
}
