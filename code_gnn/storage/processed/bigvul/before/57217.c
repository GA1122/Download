nfs4_proc_setattr(struct dentry *dentry, struct nfs_fattr *fattr,
		  struct iattr *sattr)
{
	struct inode *inode = d_inode(dentry);
	struct rpc_cred *cred = NULL;
	struct nfs4_state *state = NULL;
	struct nfs4_label *label = NULL;
	int status;

	if (pnfs_ld_layoutret_on_setattr(inode) &&
	    sattr->ia_valid & ATTR_SIZE &&
	    sattr->ia_size < i_size_read(inode))
		pnfs_commit_and_return_layout(inode);

	nfs_fattr_init(fattr);
	
	 
	if (sattr->ia_valid & ATTR_OPEN)
		sattr->ia_valid &= ~(ATTR_MTIME|ATTR_CTIME);

	 
	if ((sattr->ia_valid & ~(ATTR_FILE|ATTR_OPEN)) == 0)
		return 0;

	 
	if (sattr->ia_valid & ATTR_FILE) {
		struct nfs_open_context *ctx;

		ctx = nfs_file_open_context(sattr->ia_file);
		if (ctx) {
			cred = ctx->cred;
			state = ctx->state;
		}
	}

	label = nfs4_label_alloc(NFS_SERVER(inode), GFP_KERNEL);
	if (IS_ERR(label))
		return PTR_ERR(label);

	status = nfs4_do_setattr(inode, cred, fattr, sattr, state, NULL, label);
	if (status == 0) {
		nfs_setattr_update_inode(inode, sattr, fattr);
		nfs_setsecurity(inode, fattr, label);
	}
	nfs4_label_free(label);
	return status;
}