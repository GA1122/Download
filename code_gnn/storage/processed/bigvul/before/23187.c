static int nfs41_test_stateid(struct nfs_server *server, struct nfs4_state *state)
{
	struct nfs4_exception exception = { };
	int err;
	do {
		err = nfs4_handle_exception(server,
				_nfs41_test_stateid(server, state),
				&exception);
	} while (exception.retry);
	return err;
}