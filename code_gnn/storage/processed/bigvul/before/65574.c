nfsd4_create_session(struct svc_rqst *rqstp,
		     struct nfsd4_compound_state *cstate,
		     struct nfsd4_create_session *cr_ses)
{
	struct sockaddr *sa = svc_addr(rqstp);
	struct nfs4_client *conf, *unconf;
	struct nfs4_client *old = NULL;
	struct nfsd4_session *new;
	struct nfsd4_conn *conn;
	struct nfsd4_clid_slot *cs_slot = NULL;
	__be32 status = 0;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	if (cr_ses->flags & ~SESSION4_FLAG_MASK_A)
		return nfserr_inval;
	status = nfsd4_check_cb_sec(&cr_ses->cb_sec);
	if (status)
		return status;
	status = check_forechannel_attrs(&cr_ses->fore_channel, nn);
	if (status)
		return status;
	status = check_backchannel_attrs(&cr_ses->back_channel);
	if (status)
		goto out_release_drc_mem;
	status = nfserr_jukebox;
	new = alloc_session(&cr_ses->fore_channel, &cr_ses->back_channel);
	if (!new)
		goto out_release_drc_mem;
	conn = alloc_conn_from_crses(rqstp, cr_ses);
	if (!conn)
		goto out_free_session;

	spin_lock(&nn->client_lock);
	unconf = find_unconfirmed_client(&cr_ses->clientid, true, nn);
	conf = find_confirmed_client(&cr_ses->clientid, true, nn);
	WARN_ON_ONCE(conf && unconf);

	if (conf) {
		status = nfserr_wrong_cred;
		if (!nfsd4_mach_creds_match(conf, rqstp))
			goto out_free_conn;
		cs_slot = &conf->cl_cs_slot;
		status = check_slot_seqid(cr_ses->seqid, cs_slot->sl_seqid, 0);
		if (status) {
			if (status == nfserr_replay_cache)
				status = nfsd4_replay_create_session(cr_ses, cs_slot);
			goto out_free_conn;
		}
	} else if (unconf) {
		if (!same_creds(&unconf->cl_cred, &rqstp->rq_cred) ||
		    !rpc_cmp_addr(sa, (struct sockaddr *) &unconf->cl_addr)) {
			status = nfserr_clid_inuse;
			goto out_free_conn;
		}
		status = nfserr_wrong_cred;
		if (!nfsd4_mach_creds_match(unconf, rqstp))
			goto out_free_conn;
		cs_slot = &unconf->cl_cs_slot;
		status = check_slot_seqid(cr_ses->seqid, cs_slot->sl_seqid, 0);
		if (status) {
			 
			status = nfserr_seq_misordered;
			goto out_free_conn;
		}
		old = find_confirmed_client_by_name(&unconf->cl_name, nn);
		if (old) {
			status = mark_client_expired_locked(old);
			if (status) {
				old = NULL;
				goto out_free_conn;
			}
		}
		move_to_confirmed(unconf);
		conf = unconf;
	} else {
		status = nfserr_stale_clientid;
		goto out_free_conn;
	}
	status = nfs_ok;
	 
	cr_ses->flags &= ~SESSION4_PERSIST;
	 
	cr_ses->flags &= ~SESSION4_RDMA;

	init_session(rqstp, new, conf, cr_ses);
	nfsd4_get_session_locked(new);

	memcpy(cr_ses->sessionid.data, new->se_sessionid.data,
	       NFS4_MAX_SESSIONID_LEN);
	cs_slot->sl_seqid++;
	cr_ses->seqid = cs_slot->sl_seqid;

	 
	nfsd4_cache_create_session(cr_ses, cs_slot, status);
	spin_unlock(&nn->client_lock);
	 
	nfsd4_init_conn(rqstp, conn, new);
	nfsd4_put_session(new);
	if (old)
		expire_client(old);
	return status;
out_free_conn:
	spin_unlock(&nn->client_lock);
	free_conn(conn);
	if (old)
		expire_client(old);
out_free_session:
	__free_session(new);
out_release_drc_mem:
	nfsd4_put_drc_mem(&cr_ses->fore_channel);
	return status;
}