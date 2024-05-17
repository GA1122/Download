static void nfs4_get_lease_time_prepare(struct rpc_task *task,
					void *calldata)
{
	struct nfs4_get_lease_time_data *data =
			(struct nfs4_get_lease_time_data *)calldata;

	dprintk("--> %s\n", __func__);
	 
	nfs41_setup_sequence(data->clp->cl_session,
			&data->args->la_seq_args,
			&data->res->lr_seq_res,
			task);
	dprintk("<-- %s\n", __func__);
}