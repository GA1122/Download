nfs4_layoutget_prepare(struct rpc_task *task, void *calldata)
{
	struct nfs4_layoutget *lgp = calldata;
	struct nfs_server *server = NFS_SERVER(lgp->args.inode);
	struct nfs4_session *session = nfs4_get_session(server);

	dprintk("--> %s\n", __func__);
	 
	if (nfs41_setup_sequence(session, &lgp->args.seq_args,
				&lgp->res.seq_res, task))
		return;
	if (pnfs_choose_layoutget_stateid(&lgp->args.stateid,
					  NFS_I(lgp->args.inode)->layout,
					  &lgp->args.range,
					  lgp->args.ctx->state)) {
		rpc_exit(task, NFS4_OK);
	}
}
