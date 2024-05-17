static __be32 nfsd4_validate_stateid(struct nfs4_client *cl, stateid_t *stateid)
{
	struct nfs4_stid *s;
	__be32 status = nfserr_bad_stateid;

	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		return status;
	 
	if (!same_clid(&stateid->si_opaque.so_clid, &cl->cl_clientid)) {
		char addr_str[INET6_ADDRSTRLEN];
		rpc_ntop((struct sockaddr *)&cl->cl_addr, addr_str,
				 sizeof(addr_str));
		pr_warn_ratelimited("NFSD: client %s testing state ID "
					"with incorrect client ID\n", addr_str);
		return status;
	}
	spin_lock(&cl->cl_lock);
	s = find_stateid_locked(cl, stateid);
	if (!s)
		goto out_unlock;
	status = check_stateid_generation(stateid, &s->sc_stateid, 1);
	if (status)
		goto out_unlock;
	switch (s->sc_type) {
	case NFS4_DELEG_STID:
		status = nfs_ok;
		break;
	case NFS4_REVOKED_DELEG_STID:
		status = nfserr_deleg_revoked;
		break;
	case NFS4_OPEN_STID:
	case NFS4_LOCK_STID:
		status = nfsd4_check_openowner_confirmed(openlockstateid(s));
		break;
	default:
		printk("unknown stateid type %x\n", s->sc_type);
		 
	case NFS4_CLOSED_STID:
	case NFS4_CLOSED_DELEG_STID:
		status = nfserr_bad_stateid;
	}
out_unlock:
	spin_unlock(&cl->cl_lock);
	return status;
}