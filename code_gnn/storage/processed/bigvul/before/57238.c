int nfs4_sequence_done(struct rpc_task *task,
		       struct nfs4_sequence_res *res)
{
	return nfs40_sequence_done(task, res);
}