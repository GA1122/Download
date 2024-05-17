nfsd4_setclientid_confirm(struct svc_rqst *rqstp,
			 struct nfsd4_compound_state *cstate,
			 struct nfsd4_setclientid_confirm *setclientid_confirm)
{
	struct nfs4_client *conf, *unconf;
	struct nfs4_client *old = NULL;
	nfs4_verifier confirm = setclientid_confirm->sc_confirm; 
	clientid_t * clid = &setclientid_confirm->sc_clientid;
	__be32 status;
	struct nfsd_net	*nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	if (STALE_CLIENTID(clid, nn))
		return nfserr_stale_clientid;

	spin_lock(&nn->client_lock);
	conf = find_confirmed_client(clid, false, nn);
	unconf = find_unconfirmed_client(clid, false, nn);
	 
	status = nfserr_clid_inuse;
	if (unconf && !same_creds(&unconf->cl_cred, &rqstp->rq_cred))
		goto out;
	if (conf && !same_creds(&conf->cl_cred, &rqstp->rq_cred))
		goto out;
	 
	if (!unconf || !same_verf(&confirm, &unconf->cl_confirm)) {
		if (conf && same_verf(&confirm, &conf->cl_confirm)) {
			 
			status = nfs_ok;
		} else  
			status = nfserr_stale_clientid;
		goto out;
	}
	status = nfs_ok;
	if (conf) {  
		old = unconf;
		unhash_client_locked(old);
		nfsd4_change_callback(conf, &unconf->cl_cb_conn);
	} else {  
		old = find_confirmed_client_by_name(&unconf->cl_name, nn);
		if (old) {
			status = nfserr_clid_inuse;
			if (client_has_state(old)
					&& !same_creds(&unconf->cl_cred,
							&old->cl_cred))
				goto out;
			status = mark_client_expired_locked(old);
			if (status) {
				old = NULL;
				goto out;
			}
		}
		move_to_confirmed(unconf);
		conf = unconf;
	}
	get_client_locked(conf);
	spin_unlock(&nn->client_lock);
	nfsd4_probe_callback(conf);
	spin_lock(&nn->client_lock);
	put_client_renew_locked(conf);
out:
	spin_unlock(&nn->client_lock);
	if (old)
		expire_client(old);
	return status;
}
