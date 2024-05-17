nfs4_layoutget_prepare(struct rpc_task *task, void *calldata)
{
	struct nfs4_layoutget *lgp = calldata;
	struct nfs_server *server = NFS_SERVER(lgp->args.inode);

	dprintk("--> %s\n", __func__);
	 
	if (nfs4_setup_sequence(server, &lgp->args.seq_args,
				&lgp->res.seq_res, 0, task))
		return;
	if (pnfs_choose_layoutget_stateid(&lgp->args.stateid,
					  NFS_I(lgp->args.inode)->layout,
					  lgp->args.ctx->state)) {
		rpc_exit(task, NFS4_OK);
		return;
	}
	rpc_call_start(task);
}
