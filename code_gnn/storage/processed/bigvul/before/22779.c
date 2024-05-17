void nfs4_clear_inode(struct inode *inode)
{
	 
	nfs_inode_return_delegation_noreclaim(inode);
	 
	nfs_clear_inode(inode);
}
