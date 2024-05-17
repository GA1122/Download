static int nfs41_check_expired_locks(struct nfs4_state *state)
{
	int status, ret = -NFS4ERR_BAD_STATEID;
	struct nfs4_lock_state *lsp;
	struct nfs_server *server = NFS_SERVER(state->inode);

	list_for_each_entry(lsp, &state->lock_states, ls_locks) {
		if (test_bit(NFS_LOCK_INITIALIZED, &lsp->ls_flags)) {
			struct rpc_cred *cred = lsp->ls_state->owner->so_cred;

			status = nfs41_test_stateid(server,
					&lsp->ls_stateid,
					cred);
			trace_nfs4_test_lock_stateid(state, lsp, status);
			if (status != NFS_OK) {
				 
				if (status != -NFS4ERR_BAD_STATEID)
					nfs41_free_stateid(server,
							&lsp->ls_stateid,
							cred);
				clear_bit(NFS_LOCK_INITIALIZED, &lsp->ls_flags);
				ret = status;
			}
		}
	};

	return ret;
}