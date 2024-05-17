static struct dentry *udf_lookup(struct inode *dir, struct dentry *dentry,
				 unsigned int flags)
{
	struct inode *inode = NULL;
	struct fileIdentDesc cfi;
	struct udf_fileident_bh fibh;

	if (dentry->d_name.len > UDF_NAME_LEN - 2)
		return ERR_PTR(-ENAMETOOLONG);

#ifdef UDF_RECOVERY
	 
	if (!strncmp(dentry->d_name.name, ".B=", 3)) {
		struct kernel_lb_addr lb = {
			.logicalBlockNum = 0,
			.partitionReferenceNum =
				simple_strtoul(dentry->d_name.name + 3,
						NULL, 0),
		};
		inode = udf_iget(dir->i_sb, lb);
		if (IS_ERR(inode))
			return inode;
	} else
#endif  

	if (udf_find_entry(dir, &dentry->d_name, &fibh, &cfi)) {
		struct kernel_lb_addr loc;

		if (fibh.sbh != fibh.ebh)
			brelse(fibh.ebh);
		brelse(fibh.sbh);

		loc = lelb_to_cpu(cfi.icb.extLocation);
		inode = udf_iget(dir->i_sb, &loc);
		if (IS_ERR(inode))
			return ERR_CAST(inode);
	}

	return d_splice_alias(inode, dentry);
}