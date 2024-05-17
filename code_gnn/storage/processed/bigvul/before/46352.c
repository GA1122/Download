static int ramfs_nommu_setattr(struct dentry *dentry, struct iattr *ia)
{
	struct inode *inode = dentry->d_inode;
	unsigned int old_ia_valid = ia->ia_valid;
	int ret = 0;

	 
	ret = inode_change_ok(inode, ia);
	if (ret)
		return ret;

	 
	if (ia->ia_valid & ATTR_SIZE) {
		loff_t size = inode->i_size;

		if (ia->ia_size != size) {
			ret = ramfs_nommu_resize(inode, ia->ia_size, size);
			if (ret < 0 || ia->ia_valid == ATTR_SIZE)
				goto out;
		} else {
			 
			ia->ia_valid |= ATTR_MTIME|ATTR_CTIME;
		}
	}

	setattr_copy(inode, ia);
 out:
	ia->ia_valid = old_ia_valid;
	return ret;
}
