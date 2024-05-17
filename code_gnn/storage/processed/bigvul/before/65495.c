static __be32 lookup_clientid(clientid_t *clid,
		struct nfsd4_compound_state *cstate,
		struct nfsd_net *nn)
{
	struct nfs4_client *found;

	if (cstate->clp) {
		found = cstate->clp;
		if (!same_clid(&found->cl_clientid, clid))
			return nfserr_stale_clientid;
		return nfs_ok;
	}

	if (STALE_CLIENTID(clid, nn))
		return nfserr_stale_clientid;

	 
	WARN_ON_ONCE(cstate->session);
	spin_lock(&nn->client_lock);
	found = find_confirmed_client(clid, false, nn);
	if (!found) {
		spin_unlock(&nn->client_lock);
		return nfserr_expired;
	}
	atomic_inc(&found->cl_refcount);
	spin_unlock(&nn->client_lock);

	 
	cstate->clp = found;
	return nfs_ok;
}
