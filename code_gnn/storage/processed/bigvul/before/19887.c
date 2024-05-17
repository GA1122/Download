static int nfs4_commit_done(struct rpc_task *task, struct nfs_write_data *data)
{
	if (!nfs4_sequence_done(task, &data->res.seq_res))
		return -EAGAIN;
	return data->write_done_cb(task, data);
}