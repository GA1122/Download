static void init_session(struct svc_rqst *rqstp, struct nfsd4_session *new, struct nfs4_client *clp, struct nfsd4_create_session *cses)
{
	int idx;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	new->se_client = clp;
	gen_sessionid(new);

	INIT_LIST_HEAD(&new->se_conns);

	new->se_cb_seq_nr = 1;
	new->se_flags = cses->flags;
	new->se_cb_prog = cses->callback_prog;
	new->se_cb_sec = cses->cb_sec;
	atomic_set(&new->se_ref, 0);
	idx = hash_sessionid(&new->se_sessionid);
	list_add(&new->se_hash, &nn->sessionid_hashtbl[idx]);
	spin_lock(&clp->cl_lock);
	list_add(&new->se_perclnt, &clp->cl_sessions);
	spin_unlock(&clp->cl_lock);

	{
		struct sockaddr *sa = svc_addr(rqstp);
		 
		rpc_copy_addr((struct sockaddr *)&clp->cl_cb_conn.cb_addr, sa);
		clp->cl_cb_conn.cb_addrlen = svc_addr_len(sa);
	}
}