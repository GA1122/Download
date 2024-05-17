int ovl_permission(struct inode *inode, int mask)
{
	struct ovl_entry *oe;
	struct dentry *alias = NULL;
	struct inode *realinode;
	struct dentry *realdentry;
	bool is_upper;
	int err;

	if (S_ISDIR(inode->i_mode)) {
		oe = inode->i_private;
	} else if (mask & MAY_NOT_BLOCK) {
		return -ECHILD;
	} else {
		 
		alias = d_find_any_alias(inode);
		if (WARN_ON(!alias))
			return -ENOENT;

		oe = alias->d_fsdata;
	}

	realdentry = ovl_entry_real(oe, &is_upper);

	 
	realinode = ACCESS_ONCE(realdentry->d_inode);
	if (!realinode) {
		WARN_ON(!(mask & MAY_NOT_BLOCK));
		err = -ENOENT;
		goto out_dput;
	}

	if (mask & MAY_WRITE) {
		umode_t mode = realinode->i_mode;

		 
		err = -EROFS;
		if (is_upper && !IS_RDONLY(inode) && IS_RDONLY(realinode) &&
		    (S_ISREG(mode) || S_ISDIR(mode) || S_ISLNK(mode)))
			goto out_dput;
	}

	err = __inode_permission(realinode, mask);
out_dput:
	dput(alias);
	return err;
}
