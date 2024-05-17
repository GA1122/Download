nfsd_recall_delegations(struct list_head *reaplist)
{
	struct nfs4_client *clp;
	struct nfs4_delegation *dp, *next;

	list_for_each_entry_safe(dp, next, reaplist, dl_recall_lru) {
		list_del_init(&dp->dl_recall_lru);
		clp = dp->dl_stid.sc_client;
		 
		spin_lock(&state_lock);
		dp->dl_time = 0;
		spin_unlock(&state_lock);
		nfsd_break_one_deleg(dp);
		put_client(clp);
	}
}
