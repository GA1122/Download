static void nfs4_open_confirm_prepare(struct rpc_task *task, void *calldata)
{
	struct nfs4_opendata *data = calldata;

	nfs40_setup_sequence(data->o_arg.server->nfs_client->cl_slot_tbl,
			     &data->c_arg.seq_args, &data->c_res.seq_res, task);
}