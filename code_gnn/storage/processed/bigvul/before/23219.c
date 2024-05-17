static int nfs4_open_recover(struct nfs4_opendata *opendata, struct nfs4_state *state)
{
	struct nfs4_state *newstate;
	int ret;

	 
	clear_bit(NFS_DELEGATED_STATE, &state->flags);
	smp_rmb();
	if (state->n_rdwr != 0) {
		clear_bit(NFS_O_RDWR_STATE, &state->flags);
		ret = nfs4_open_recover_helper(opendata, FMODE_READ|FMODE_WRITE, &newstate);
		if (ret != 0)
			return ret;
		if (newstate != state)
			return -ESTALE;
	}
	if (state->n_wronly != 0) {
		clear_bit(NFS_O_WRONLY_STATE, &state->flags);
		ret = nfs4_open_recover_helper(opendata, FMODE_WRITE, &newstate);
		if (ret != 0)
			return ret;
		if (newstate != state)
			return -ESTALE;
	}
	if (state->n_rdonly != 0) {
		clear_bit(NFS_O_RDONLY_STATE, &state->flags);
		ret = nfs4_open_recover_helper(opendata, FMODE_READ, &newstate);
		if (ret != 0)
			return ret;
		if (newstate != state)
			return -ESTALE;
	}
	 
	if (test_bit(NFS_DELEGATED_STATE, &state->flags) == 0 &&
	    memcmp(state->stateid.data, state->open_stateid.data, sizeof(state->stateid.data)) != 0) {
		write_seqlock(&state->seqlock);
		if (test_bit(NFS_DELEGATED_STATE, &state->flags) == 0)
			memcpy(state->stateid.data, state->open_stateid.data, sizeof(state->stateid.data));
		write_sequnlock(&state->seqlock);
	}
	return 0;
}