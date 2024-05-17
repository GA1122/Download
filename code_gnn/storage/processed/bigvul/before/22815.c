nfs_setattr(struct dentry *dentry, struct iattr *attr)
{
	struct inode *inode = dentry->d_inode;
	struct nfs_fattr fattr;
	int error;

	nfs_inc_stats(inode, NFSIOS_VFSSETATTR);

	 
	if (attr->ia_valid & (ATTR_KILL_SUID | ATTR_KILL_SGID))
		attr->ia_valid &= ~ATTR_MODE;

	if (attr->ia_valid & ATTR_SIZE) {
		if (!S_ISREG(inode->i_mode) || attr->ia_size == i_size_read(inode))
			attr->ia_valid &= ~ATTR_SIZE;
	}

	 
	attr->ia_valid &= NFS_VALID_ATTRS;
	if ((attr->ia_valid & ~ATTR_FILE) == 0)
		return 0;

	 
	if (S_ISREG(inode->i_mode)) {
		filemap_write_and_wait(inode->i_mapping);
		nfs_wb_all(inode);
	}
	 
	if ((attr->ia_valid & (ATTR_MODE|ATTR_UID|ATTR_GID)) != 0)
		nfs_inode_return_delegation(inode);
	error = NFS_PROTO(inode)->setattr(dentry, &fattr, attr);
	if (error == 0)
		nfs_refresh_inode(inode, &fattr);
	return error;
}