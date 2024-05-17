xfs_inode_free(
	struct xfs_inode	*ip)
{
	ASSERT(!xfs_isiflocked(ip));

	 
	spin_lock(&ip->i_flags_lock);
	ip->i_flags = XFS_IRECLAIM;
	ip->i_ino = 0;
	spin_unlock(&ip->i_flags_lock);

	__xfs_inode_free(ip);
}
