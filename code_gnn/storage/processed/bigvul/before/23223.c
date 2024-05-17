static void nfs4_proc_commit_setup(struct nfs_write_data *data, struct rpc_message *msg)
{
	struct nfs_server *server = NFS_SERVER(data->inode);

	if (data->lseg) {
		data->args.bitmask = NULL;
		data->res.fattr = NULL;
	} else
		data->args.bitmask = server->cache_consistency_bitmask;
	if (!data->write_done_cb)
		data->write_done_cb = nfs4_commit_done_cb;
	data->res.server = server;
	msg->rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_COMMIT];
}