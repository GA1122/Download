static int nfs40_open_expired(struct nfs4_state_owner *sp, struct nfs4_state *state)
{
	 
	nfs40_clear_delegation_stateid(state);
	return nfs4_open_expired(sp, state);
}