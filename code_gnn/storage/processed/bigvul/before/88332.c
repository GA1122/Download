xfs_inode_supports_dax(
	struct xfs_inode	*ip)
{
	struct xfs_mount	*mp = ip->i_mount;

	 
	if (!S_ISREG(VFS_I(ip)->i_mode) || xfs_is_reflink_inode(ip))
		return false;

	 
	if (!(mp->m_flags & XFS_MOUNT_DAX) &&
	    !(ip->i_d.di_flags2 & XFS_DIFLAG2_DAX))
		return false;

	 
	if (mp->m_sb.sb_blocksize != PAGE_SIZE)
		return false;

	 
	return xfs_find_daxdev_for_inode(VFS_I(ip)) != NULL;
}