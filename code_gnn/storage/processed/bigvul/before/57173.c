static int nfs4_open_recover(struct nfs4_opendata *opendata, struct nfs4_state *state)
{
	struct nfs4_state *newstate;
	int ret;

	 
	clear_bit(NFS_O_RDWR_STATE, &state->flags);
	clear_bit(NFS_O_WRONLY_STATE, &state->flags);
	clear_bit(NFS_O_RDONLY_STATE, &state->flags);
	 
	clear_bit(NFS_DELEGATED_STATE, &state->flags);
	clear_bit(NFS_OPEN_STATE, &state->flags);
	smp_rmb();
	if (state->n_rdwr != 0) {
		ret = nfs4_open_recover_helper(opendata, FMODE_READ|FMODE_WRITE, &newstate);
		if (ret != 0)
			return ret;
		if (newstate != state)
			return -ESTALE;
	}
	if (state->n_wronly != 0) {
		ret = nfs4_open_recover_helper(opendata, FMODE_WRITE, &newstate);
		if (ret != 0)
			return ret;
		if (newstate != state)
			return -ESTALE;
	}
	if (state->n_rdonly != 0) {
		ret = nfs4_open_recover_helper(opendata, FMODE_READ, &newstate);
		if (ret != 0)
			return ret;
		if (newstate != state)
			return -ESTALE;
	}
	 
	if (test_bit(NFS_DELEGATED_STATE, &state->flags) == 0 &&
	    !nfs4_stateid_match(&state->stateid, &state->open_stateid)) {
		write_seqlock(&state->seqlock);
		if (test_bit(NFS_DELEGATED_STATE, &state->flags) == 0)
			nfs4_stateid_copy(&state->stateid, &state->open_stateid);
		write_sequnlock(&state->seqlock);
	}
	return 0;
}