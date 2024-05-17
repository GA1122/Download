static void nfs4_close_done(struct rpc_task *task, void *data)
{
	struct nfs4_closedata *calldata = data;
	struct nfs4_state *state = calldata->state;
	struct nfs_server *server = NFS_SERVER(calldata->inode);
	nfs4_stateid *res_stateid = NULL;

	dprintk("%s: begin!\n", __func__);
	if (!nfs4_sequence_done(task, &calldata->res.seq_res))
		return;
	trace_nfs4_close(state, &calldata->arg, &calldata->res, task->tk_status);
         
	switch (task->tk_status) {
		case 0:
			res_stateid = &calldata->res.stateid;
			if (calldata->arg.fmode == 0 && calldata->roc)
				pnfs_roc_set_barrier(state->inode,
						     calldata->roc_barrier);
			renew_lease(server, calldata->timestamp);
			break;
		case -NFS4ERR_ADMIN_REVOKED:
		case -NFS4ERR_STALE_STATEID:
		case -NFS4ERR_OLD_STATEID:
		case -NFS4ERR_BAD_STATEID:
		case -NFS4ERR_EXPIRED:
			if (!nfs4_stateid_match(&calldata->arg.stateid,
						&state->open_stateid)) {
				rpc_restart_call_prepare(task);
				goto out_release;
			}
			if (calldata->arg.fmode == 0)
				break;
		default:
			if (nfs4_async_handle_error(task, server, state, NULL) == -EAGAIN) {
				rpc_restart_call_prepare(task);
				goto out_release;
			}
	}
	nfs_clear_open_stateid(state, res_stateid, calldata->arg.fmode);
out_release:
	nfs_release_seqid(calldata->arg.seqid);
	nfs_refresh_inode(calldata->inode, calldata->res.fattr);
	dprintk("%s: done, ret = %d!\n", __func__, task->tk_status);
}