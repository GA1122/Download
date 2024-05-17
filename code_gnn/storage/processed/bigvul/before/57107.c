static void nfs4_close_prepare(struct rpc_task *task, void *data)
{
	struct nfs4_closedata *calldata = data;
	struct nfs4_state *state = calldata->state;
	struct inode *inode = calldata->inode;
	bool is_rdonly, is_wronly, is_rdwr;
	int call_close = 0;

	dprintk("%s: begin!\n", __func__);
	if (nfs_wait_on_sequence(calldata->arg.seqid, task) != 0)
		goto out_wait;

	task->tk_msg.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_OPEN_DOWNGRADE];
	spin_lock(&state->owner->so_lock);
	is_rdwr = test_bit(NFS_O_RDWR_STATE, &state->flags);
	is_rdonly = test_bit(NFS_O_RDONLY_STATE, &state->flags);
	is_wronly = test_bit(NFS_O_WRONLY_STATE, &state->flags);
	nfs4_stateid_copy(&calldata->arg.stateid, &state->open_stateid);
	 
	calldata->arg.fmode = 0;
	if (state->n_rdwr == 0) {
		if (state->n_rdonly == 0)
			call_close |= is_rdonly;
		else if (is_rdonly)
			calldata->arg.fmode |= FMODE_READ;
		if (state->n_wronly == 0)
			call_close |= is_wronly;
		else if (is_wronly)
			calldata->arg.fmode |= FMODE_WRITE;
	} else if (is_rdwr)
		calldata->arg.fmode |= FMODE_READ|FMODE_WRITE;

	if (calldata->arg.fmode == 0)
		call_close |= is_rdwr;

	if (!nfs4_valid_open_stateid(state))
		call_close = 0;
	spin_unlock(&state->owner->so_lock);

	if (!call_close) {
		 
		goto out_no_action;
	}

	if (calldata->arg.fmode == 0) {
		task->tk_msg.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_CLOSE];
		if (calldata->roc &&
		    pnfs_roc_drain(inode, &calldata->roc_barrier, task)) {
			nfs_release_seqid(calldata->arg.seqid);
			goto out_wait;
		    }
	}
	calldata->arg.share_access =
		nfs4_map_atomic_open_share(NFS_SERVER(inode),
				calldata->arg.fmode, 0);

	nfs_fattr_init(calldata->res.fattr);
	calldata->timestamp = jiffies;
	if (nfs4_setup_sequence(NFS_SERVER(inode),
				&calldata->arg.seq_args,
				&calldata->res.seq_res,
				task) != 0)
		nfs_release_seqid(calldata->arg.seqid);
	dprintk("%s: done!\n", __func__);
	return;
out_no_action:
	task->tk_action = NULL;
out_wait:
	nfs4_sequence_done(task, &calldata->res.seq_res);
}