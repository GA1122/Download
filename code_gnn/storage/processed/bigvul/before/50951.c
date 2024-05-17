static bool is_mnt_ns_file(struct dentry *dentry)
{
	 
	return dentry->d_op == &ns_dentry_operations &&
	       dentry->d_fsdata == &mntns_operations;
}
