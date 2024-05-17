nfsd4_lm_notify(struct file_lock *fl)
{
	struct nfs4_lockowner		*lo = (struct nfs4_lockowner *)fl->fl_owner;
	struct net			*net = lo->lo_owner.so_client->net;
	struct nfsd_net			*nn = net_generic(net, nfsd_net_id);
	struct nfsd4_blocked_lock	*nbl = container_of(fl,
						struct nfsd4_blocked_lock, nbl_lock);
	bool queue = false;

	 
	spin_lock(&nn->blocked_locks_lock);
	if (!list_empty(&nbl->nbl_list)) {
		list_del_init(&nbl->nbl_list);
		list_del_init(&nbl->nbl_lru);
		queue = true;
	}
	spin_unlock(&nn->blocked_locks_lock);

	if (queue)
		nfsd4_run_cb(&nbl->nbl_cb);
}