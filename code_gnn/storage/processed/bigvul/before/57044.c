static int _nfs4_open_and_get_state(struct nfs4_opendata *opendata,
		fmode_t fmode,
		int flags,
		struct nfs_open_context *ctx)
{
	struct nfs4_state_owner *sp = opendata->owner;
	struct nfs_server *server = sp->so_server;
	struct dentry *dentry;
	struct nfs4_state *state;
	unsigned int seq;
	int ret;

	seq = raw_seqcount_begin(&sp->so_reclaim_seqcount);

	ret = _nfs4_proc_open(opendata);
	if (ret != 0)
		goto out;

	state = nfs4_opendata_to_nfs4_state(opendata);
	ret = PTR_ERR(state);
	if (IS_ERR(state))
		goto out;
	if (server->caps & NFS_CAP_POSIX_LOCK)
		set_bit(NFS_STATE_POSIX_LOCKS, &state->flags);

	dentry = opendata->dentry;
	if (d_really_is_negative(dentry)) {
		 
		d_drop(dentry);
		dentry = d_add_unique(dentry, igrab(state->inode));
		if (dentry == NULL) {
			dentry = opendata->dentry;
		} else if (dentry != ctx->dentry) {
			dput(ctx->dentry);
			ctx->dentry = dget(dentry);
		}
		nfs_set_verifier(dentry,
				nfs_save_change_attribute(d_inode(opendata->dir)));
	}

	ret = nfs4_opendata_access(sp->so_cred, opendata, state, fmode, flags);
	if (ret != 0)
		goto out;

	ctx->state = state;
	if (d_inode(dentry) == state->inode) {
		nfs_inode_attach_open_context(ctx);
		if (read_seqcount_retry(&sp->so_reclaim_seqcount, seq))
			nfs4_schedule_stateid_recovery(server, state);
	}
out:
	return ret;
}