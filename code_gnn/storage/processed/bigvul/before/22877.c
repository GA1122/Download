static int nfs4_lock_reclaim(struct nfs4_state *state, struct file_lock *request)
{
	struct nfs_server *server = NFS_SERVER(state->inode);
	struct nfs4_exception exception = { };
	int err;

	do {
		 
		if (test_bit(NFS_DELEGATED_STATE, &state->flags) != 0)
			return 0;
		err = _nfs4_do_setlk(state, F_SETLK, request, 1);
		if (err != -NFS4ERR_DELAY)
			break;
		nfs4_handle_exception(server, err, &exception);
	} while (exception.retry);
	return err;
}