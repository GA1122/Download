static void nfs4_layoutget_done(struct rpc_task *task, void *calldata)
{
	struct nfs4_layoutget *lgp = calldata;
	struct inode *inode = lgp->args.inode;
	struct nfs_server *server = NFS_SERVER(inode);
	struct pnfs_layout_hdr *lo;
	struct nfs4_state *state = NULL;
	unsigned long timeo, now, giveup;

	dprintk("--> %s tk_status => %d\n", __func__, -task->tk_status);

	if (!nfs41_sequence_done(task, &lgp->res.seq_res))
		goto out;

	switch (task->tk_status) {
	case 0:
		goto out;
	 
	case -NFS4ERR_LAYOUTTRYLATER:
	 
	case -NFS4ERR_RECALLCONFLICT:
		timeo = rpc_get_timeout(task->tk_client);
		giveup = lgp->args.timestamp + timeo;
		now = jiffies;
		if (time_after(giveup, now)) {
			unsigned long delay;

			 
			delay = max_t(unsigned long, NFS4_POLL_RETRY_MIN,
				    min((giveup - now - 1),
					now - lgp->args.timestamp));

			dprintk("%s: NFS4ERR_RECALLCONFLICT waiting %lu\n",
				__func__, delay);
			rpc_delay(task, delay);
			task->tk_status = 0;
			rpc_restart_call_prepare(task);
			goto out;  
		}
		break;
	case -NFS4ERR_EXPIRED:
	case -NFS4ERR_BAD_STATEID:
		spin_lock(&inode->i_lock);
		lo = NFS_I(inode)->layout;
		if (!lo || list_empty(&lo->plh_segs)) {
			spin_unlock(&inode->i_lock);
			 
			state = lgp->args.ctx->state;
		} else {
			LIST_HEAD(head);

			 
			pnfs_mark_matching_lsegs_invalid(lo, &head, NULL);
			spin_unlock(&inode->i_lock);
			pnfs_free_lseg_list(&head);
	
			task->tk_status = 0;
			rpc_restart_call_prepare(task);
		}
	}
	if (nfs4_async_handle_error(task, server, state, NULL) == -EAGAIN)
		rpc_restart_call_prepare(task);
out:
	dprintk("<-- %s\n", __func__);
}