static int _nfs4_do_open(struct inode *dir,
			struct nfs_open_context *ctx,
			int flags,
			struct iattr *sattr,
			struct nfs4_label *label,
			int *opened)
{
	struct nfs4_state_owner  *sp;
	struct nfs4_state     *state = NULL;
	struct nfs_server       *server = NFS_SERVER(dir);
	struct nfs4_opendata *opendata;
	struct dentry *dentry = ctx->dentry;
	struct rpc_cred *cred = ctx->cred;
	struct nfs4_threshold **ctx_th = &ctx->mdsthreshold;
	fmode_t fmode = ctx->mode & (FMODE_READ|FMODE_WRITE|FMODE_EXEC);
	enum open_claim_type4 claim = NFS4_OPEN_CLAIM_NULL;
	struct nfs4_label *olabel = NULL;
	int status;

	 
	status = -ENOMEM;
	sp = nfs4_get_state_owner(server, cred, GFP_KERNEL);
	if (sp == NULL) {
		dprintk("nfs4_do_open: nfs4_get_state_owner failed!\n");
		goto out_err;
	}
	status = nfs4_recover_expired_lease(server);
	if (status != 0)
		goto err_put_state_owner;
	if (d_really_is_positive(dentry))
		nfs4_return_incompatible_delegation(d_inode(dentry), fmode);
	status = -ENOMEM;
	if (d_really_is_positive(dentry))
		claim = NFS4_OPEN_CLAIM_FH;
	opendata = nfs4_opendata_alloc(dentry, sp, fmode, flags, sattr,
			label, claim, GFP_KERNEL);
	if (opendata == NULL)
		goto err_put_state_owner;

	if (label) {
		olabel = nfs4_label_alloc(server, GFP_KERNEL);
		if (IS_ERR(olabel)) {
			status = PTR_ERR(olabel);
			goto err_opendata_put;
		}
	}

	if (server->attr_bitmask[2] & FATTR4_WORD2_MDSTHRESHOLD) {
		if (!opendata->f_attr.mdsthreshold) {
			opendata->f_attr.mdsthreshold = pnfs_mdsthreshold_alloc();
			if (!opendata->f_attr.mdsthreshold)
				goto err_free_label;
		}
		opendata->o_arg.open_bitmap = &nfs4_pnfs_open_bitmap[0];
	}
	if (d_really_is_positive(dentry))
		opendata->state = nfs4_get_open_state(d_inode(dentry), sp);

	status = _nfs4_open_and_get_state(opendata, fmode, flags, ctx);
	if (status != 0)
		goto err_free_label;
	state = ctx->state;

	if ((opendata->o_arg.open_flags & (O_CREAT|O_EXCL)) == (O_CREAT|O_EXCL) &&
	    (opendata->o_arg.createmode != NFS4_CREATE_GUARDED)) {
		nfs4_exclusive_attrset(opendata, sattr);

		nfs_fattr_init(opendata->o_res.f_attr);
		status = nfs4_do_setattr(state->inode, cred,
				opendata->o_res.f_attr, sattr,
				state, label, olabel);
		if (status == 0) {
			nfs_setattr_update_inode(state->inode, sattr,
					opendata->o_res.f_attr);
			nfs_setsecurity(state->inode, opendata->o_res.f_attr, olabel);
		}
	}
	if (opendata->file_created)
		*opened |= FILE_CREATED;

	if (pnfs_use_threshold(ctx_th, opendata->f_attr.mdsthreshold, server)) {
		*ctx_th = opendata->f_attr.mdsthreshold;
		opendata->f_attr.mdsthreshold = NULL;
	}

	nfs4_label_free(olabel);

	nfs4_opendata_put(opendata);
	nfs4_put_state_owner(sp);
	return 0;
err_free_label:
	nfs4_label_free(olabel);
err_opendata_put:
	nfs4_opendata_put(opendata);
err_put_state_owner:
	nfs4_put_state_owner(sp);
out_err:
	return status;
}