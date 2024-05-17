nfsd4_release_lockowner(struct svc_rqst *rqstp,
			struct nfsd4_compound_state *cstate,
			struct nfsd4_release_lockowner *rlockowner)
{
	clientid_t *clid = &rlockowner->rl_clientid;
	struct nfs4_stateowner *sop;
	struct nfs4_lockowner *lo = NULL;
	struct nfs4_ol_stateid *stp;
	struct xdr_netobj *owner = &rlockowner->rl_owner;
	unsigned int hashval = ownerstr_hashval(owner);
	__be32 status;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
	struct nfs4_client *clp;
	LIST_HEAD (reaplist);

	dprintk("nfsd4_release_lockowner clientid: (%08x/%08x):\n",
		clid->cl_boot, clid->cl_id);

	status = lookup_clientid(clid, cstate, nn);
	if (status)
		return status;

	clp = cstate->clp;
	 
	spin_lock(&clp->cl_lock);
	list_for_each_entry(sop, &clp->cl_ownerstr_hashtbl[hashval],
			    so_strhash) {

		if (sop->so_is_open_owner || !same_owner_str(sop, owner))
			continue;

		 
		lo = lockowner(sop);
		list_for_each_entry(stp, &sop->so_stateids, st_perstateowner) {
			if (check_for_locks(stp->st_stid.sc_file, lo)) {
				status = nfserr_locks_held;
				spin_unlock(&clp->cl_lock);
				return status;
			}
		}

		nfs4_get_stateowner(sop);
		break;
	}
	if (!lo) {
		spin_unlock(&clp->cl_lock);
		return status;
	}

	unhash_lockowner_locked(lo);
	while (!list_empty(&lo->lo_owner.so_stateids)) {
		stp = list_first_entry(&lo->lo_owner.so_stateids,
				       struct nfs4_ol_stateid,
				       st_perstateowner);
		WARN_ON(!unhash_lock_stateid(stp));
		put_ol_stateid_locked(stp, &reaplist);
	}
	spin_unlock(&clp->cl_lock);
	free_ol_stateid_reaplist(&reaplist);
	nfs4_put_stateowner(&lo->lo_owner);

	return status;
}