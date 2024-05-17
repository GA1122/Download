nfsd4_exchange_id(struct svc_rqst *rqstp,
		  struct nfsd4_compound_state *cstate,
		  struct nfsd4_exchange_id *exid)
{
	struct nfs4_client *conf, *new;
	struct nfs4_client *unconf = NULL;
	__be32 status;
	char			addr_str[INET6_ADDRSTRLEN];
	nfs4_verifier		verf = exid->verifier;
	struct sockaddr		*sa = svc_addr(rqstp);
	bool	update = exid->flags & EXCHGID4_FLAG_UPD_CONFIRMED_REC_A;
	struct nfsd_net		*nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	rpc_ntop(sa, addr_str, sizeof(addr_str));
	dprintk("%s rqstp=%p exid=%p clname.len=%u clname.data=%p "
		"ip_addr=%s flags %x, spa_how %d\n",
		__func__, rqstp, exid, exid->clname.len, exid->clname.data,
		addr_str, exid->flags, exid->spa_how);

	if (exid->flags & ~EXCHGID4_FLAG_MASK_A)
		return nfserr_inval;

	new = create_client(exid->clname, rqstp, &verf);
	if (new == NULL)
		return nfserr_jukebox;

	switch (exid->spa_how) {
	case SP4_MACH_CRED:
		exid->spo_must_enforce[0] = 0;
		exid->spo_must_enforce[1] = (
			1 << (OP_BIND_CONN_TO_SESSION - 32) |
			1 << (OP_EXCHANGE_ID - 32) |
			1 << (OP_CREATE_SESSION - 32) |
			1 << (OP_DESTROY_SESSION - 32) |
			1 << (OP_DESTROY_CLIENTID - 32));

		exid->spo_must_allow[0] &= (1 << (OP_CLOSE) |
					1 << (OP_OPEN_DOWNGRADE) |
					1 << (OP_LOCKU) |
					1 << (OP_DELEGRETURN));

		exid->spo_must_allow[1] &= (
					1 << (OP_TEST_STATEID - 32) |
					1 << (OP_FREE_STATEID - 32));
		if (!svc_rqst_integrity_protected(rqstp)) {
			status = nfserr_inval;
			goto out_nolock;
		}
		 
		if (!new->cl_cred.cr_principal &&
					!new->cl_cred.cr_raw_principal) {
			status = nfserr_serverfault;
			goto out_nolock;
		}
		new->cl_mach_cred = true;
	case SP4_NONE:
		break;
	default:				 
		WARN_ON_ONCE(1);
	case SP4_SSV:
		status = nfserr_encr_alg_unsupp;
		goto out_nolock;
	}

	 
	spin_lock(&nn->client_lock);
	conf = find_confirmed_client_by_name(&exid->clname, nn);
	if (conf) {
		bool creds_match = same_creds(&conf->cl_cred, &rqstp->rq_cred);
		bool verfs_match = same_verf(&verf, &conf->cl_verifier);

		if (update) {
			if (!clp_used_exchangeid(conf)) {  
				status = nfserr_inval;
				goto out;
			}
			if (!nfsd4_mach_creds_match(conf, rqstp)) {
				status = nfserr_wrong_cred;
				goto out;
			}
			if (!creds_match) {  
				status = nfserr_perm;
				goto out;
			}
			if (!verfs_match) {  
				status = nfserr_not_same;
				goto out;
			}
			 
			exid->flags |= EXCHGID4_FLAG_CONFIRMED_R;
			goto out_copy;
		}
		if (!creds_match) {  
			if (client_has_state(conf)) {
				status = nfserr_clid_inuse;
				goto out;
			}
			goto out_new;
		}
		if (verfs_match) {  
			conf->cl_exchange_flags |= EXCHGID4_FLAG_CONFIRMED_R;
			goto out_copy;
		}
		 
		conf = NULL;
		goto out_new;
	}

	if (update) {  
		status = nfserr_noent;
		goto out;
	}

	unconf  = find_unconfirmed_client_by_name(&exid->clname, nn);
	if (unconf)  
		unhash_client_locked(unconf);

	 
out_new:
	if (conf) {
		status = mark_client_expired_locked(conf);
		if (status)
			goto out;
	}
	new->cl_minorversion = cstate->minorversion;
	new->cl_spo_must_allow.u.words[0] = exid->spo_must_allow[0];
	new->cl_spo_must_allow.u.words[1] = exid->spo_must_allow[1];

	gen_clid(new, nn);
	add_to_unconfirmed(new);
	swap(new, conf);
out_copy:
	exid->clientid.cl_boot = conf->cl_clientid.cl_boot;
	exid->clientid.cl_id = conf->cl_clientid.cl_id;

	exid->seqid = conf->cl_cs_slot.sl_seqid + 1;
	nfsd4_set_ex_flags(conf, exid);

	dprintk("nfsd4_exchange_id seqid %d flags %x\n",
		conf->cl_cs_slot.sl_seqid, conf->cl_exchange_flags);
	status = nfs_ok;

out:
	spin_unlock(&nn->client_lock);
out_nolock:
	if (new)
		expire_client(new);
	if (unconf)
		expire_client(unconf);
	return status;
}