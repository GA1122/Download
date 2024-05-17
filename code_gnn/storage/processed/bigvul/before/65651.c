static u64 nfsd_find_all_delegations(struct nfs4_client *clp, u64 max,
				     struct list_head *victims)
{
	struct nfs4_delegation *dp, *next;
	struct nfsd_net *nn = net_generic(current->nsproxy->net_ns,
						nfsd_net_id);
	u64 count = 0;

	lockdep_assert_held(&nn->client_lock);

	spin_lock(&state_lock);
	list_for_each_entry_safe(dp, next, &clp->cl_delegations, dl_perclnt) {
		if (victims) {
			 
			if (dp->dl_time != 0)
				continue;

			atomic_inc(&clp->cl_refcount);
			WARN_ON(!unhash_delegation_locked(dp));
			list_add(&dp->dl_recall_lru, victims);
		}
		++count;
		 
		WARN_ON_ONCE(count == (INT_MAX / 2));
		if (count == max)
			break;
	}
	spin_unlock(&state_lock);
	return count;
}
