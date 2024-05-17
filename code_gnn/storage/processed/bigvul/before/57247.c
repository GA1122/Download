static int nfs4_setup_sequence(const struct nfs_server *server,
			       struct nfs4_sequence_args *args,
			       struct nfs4_sequence_res *res,
			       struct rpc_task *task)
{
	struct nfs4_session *session = nfs4_get_session(server);
	int ret = 0;

	if (!session)
		return nfs40_setup_sequence(server->nfs_client->cl_slot_tbl,
					    args, res, task);

	dprintk("--> %s clp %p session %p sr_slot %u\n",
		__func__, session->clp, session, res->sr_slot ?
			res->sr_slot->slot_nr : NFS4_NO_SLOT);

	ret = nfs41_setup_sequence(session, args, res, task);

	dprintk("<-- %s status=%d\n", __func__, ret);
	return ret;
}
