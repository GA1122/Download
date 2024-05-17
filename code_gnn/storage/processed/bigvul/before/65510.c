nfs4_check_open_reclaim(clientid_t *clid,
		struct nfsd4_compound_state *cstate,
		struct nfsd_net *nn)
{
	__be32 status;

	 
	status = lookup_clientid(clid, cstate, nn);
	if (status)
		return nfserr_reclaim_bad;

	if (test_bit(NFSD4_CLIENT_RECLAIM_COMPLETE, &cstate->clp->cl_flags))
		return nfserr_no_grace;

	if (nfsd4_client_record_check(cstate->clp))
		return nfserr_reclaim_bad;

	return nfs_ok;
}