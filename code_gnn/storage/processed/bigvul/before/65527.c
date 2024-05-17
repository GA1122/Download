nfs4_laundromat(struct nfsd_net *nn)
{
	struct nfs4_client *clp;
	struct nfs4_openowner *oo;
	struct nfs4_delegation *dp;
	struct nfs4_ol_stateid *stp;
	struct nfsd4_blocked_lock *nbl;
	struct list_head *pos, *next, reaplist;
	time_t cutoff = get_seconds() - nn->nfsd4_lease;
	time_t t, new_timeo = nn->nfsd4_lease;

	dprintk("NFSD: laundromat service - starting\n");
	nfsd4_end_grace(nn);
	INIT_LIST_HEAD(&reaplist);
	spin_lock(&nn->client_lock);
	list_for_each_safe(pos, next, &nn->client_lru) {
		clp = list_entry(pos, struct nfs4_client, cl_lru);
		if (time_after((unsigned long)clp->cl_time, (unsigned long)cutoff)) {
			t = clp->cl_time - cutoff;
			new_timeo = min(new_timeo, t);
			break;
		}
		if (mark_client_expired_locked(clp)) {
			dprintk("NFSD: client in use (clientid %08x)\n",
				clp->cl_clientid.cl_id);
			continue;
		}
		list_add(&clp->cl_lru, &reaplist);
	}
	spin_unlock(&nn->client_lock);
	list_for_each_safe(pos, next, &reaplist) {
		clp = list_entry(pos, struct nfs4_client, cl_lru);
		dprintk("NFSD: purging unused client (clientid %08x)\n",
			clp->cl_clientid.cl_id);
		list_del_init(&clp->cl_lru);
		expire_client(clp);
	}
	spin_lock(&state_lock);
	list_for_each_safe(pos, next, &nn->del_recall_lru) {
		dp = list_entry (pos, struct nfs4_delegation, dl_recall_lru);
		if (time_after((unsigned long)dp->dl_time, (unsigned long)cutoff)) {
			t = dp->dl_time - cutoff;
			new_timeo = min(new_timeo, t);
			break;
		}
		WARN_ON(!unhash_delegation_locked(dp));
		list_add(&dp->dl_recall_lru, &reaplist);
	}
	spin_unlock(&state_lock);
	while (!list_empty(&reaplist)) {
		dp = list_first_entry(&reaplist, struct nfs4_delegation,
					dl_recall_lru);
		list_del_init(&dp->dl_recall_lru);
		revoke_delegation(dp);
	}

	spin_lock(&nn->client_lock);
	while (!list_empty(&nn->close_lru)) {
		oo = list_first_entry(&nn->close_lru, struct nfs4_openowner,
					oo_close_lru);
		if (time_after((unsigned long)oo->oo_time,
			       (unsigned long)cutoff)) {
			t = oo->oo_time - cutoff;
			new_timeo = min(new_timeo, t);
			break;
		}
		list_del_init(&oo->oo_close_lru);
		stp = oo->oo_last_closed_stid;
		oo->oo_last_closed_stid = NULL;
		spin_unlock(&nn->client_lock);
		nfs4_put_stid(&stp->st_stid);
		spin_lock(&nn->client_lock);
	}
	spin_unlock(&nn->client_lock);

	 
	BUG_ON(!list_empty(&reaplist));
	spin_lock(&nn->blocked_locks_lock);
	while (!list_empty(&nn->blocked_locks_lru)) {
		nbl = list_first_entry(&nn->blocked_locks_lru,
					struct nfsd4_blocked_lock, nbl_lru);
		if (time_after((unsigned long)nbl->nbl_time,
			       (unsigned long)cutoff)) {
			t = nbl->nbl_time - cutoff;
			new_timeo = min(new_timeo, t);
			break;
		}
		list_move(&nbl->nbl_lru, &reaplist);
		list_del_init(&nbl->nbl_list);
	}
	spin_unlock(&nn->blocked_locks_lock);

	while (!list_empty(&reaplist)) {
		nbl = list_first_entry(&nn->blocked_locks_lru,
					struct nfsd4_blocked_lock, nbl_lru);
		list_del_init(&nbl->nbl_lru);
		posix_unblock_lock(&nbl->nbl_lock);
		free_blocked_lock(nbl);
	}

	new_timeo = max_t(time_t, new_timeo, NFSD_LAUNDROMAT_MINTIMEOUT);
	return new_timeo;
}