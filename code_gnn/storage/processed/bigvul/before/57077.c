static void nfs41_check_delegation_stateid(struct nfs4_state *state)
{
	struct nfs_server *server = NFS_SERVER(state->inode);
	nfs4_stateid stateid;
	struct nfs_delegation *delegation;
	struct rpc_cred *cred;
	int status;

	 
	rcu_read_lock();
	delegation = rcu_dereference(NFS_I(state->inode)->delegation);
	if (delegation == NULL) {
		rcu_read_unlock();
		return;
	}

	nfs4_stateid_copy(&stateid, &delegation->stateid);
	cred = get_rpccred(delegation->cred);
	rcu_read_unlock();
	status = nfs41_test_stateid(server, &stateid, cred);
	trace_nfs4_test_delegation_stateid(state, NULL, status);

	if (status != NFS_OK) {
		 
		if (status != -NFS4ERR_BAD_STATEID)
			nfs41_free_stateid(server, &stateid, cred);
		nfs_finish_clear_delegation_stateid(state);
	}

	put_rpccred(cred);
}