xfs_inode_ag_walk_grab(
	struct xfs_inode	*ip,
	int			flags)
{
	struct inode		*inode = VFS_I(ip);
	bool			newinos = !!(flags & XFS_AGITER_INEW_WAIT);

	ASSERT(rcu_read_lock_held());

	 
	spin_lock(&ip->i_flags_lock);
	if (!ip->i_ino)
		goto out_unlock_noent;

	 
	if ((!newinos && __xfs_iflags_test(ip, XFS_INEW)) ||
	    __xfs_iflags_test(ip, XFS_IRECLAIMABLE | XFS_IRECLAIM))
		goto out_unlock_noent;
	spin_unlock(&ip->i_flags_lock);

	 
	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		return -EFSCORRUPTED;

	 
	if (!igrab(inode))
		return -ENOENT;

	 
	return 0;

out_unlock_noent:
	spin_unlock(&ip->i_flags_lock);
	return -ENOENT;
}
