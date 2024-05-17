static void __ip_vs_del_dest(struct net *net, struct ip_vs_dest *dest)
{
	struct netns_ipvs *ipvs = net_ipvs(net);

	ip_vs_stop_estimator(net, &dest->stats);

	 
	write_lock_bh(&ipvs->rs_lock);
	ip_vs_rs_unhash(dest);
	write_unlock_bh(&ipvs->rs_lock);

	 
	if (atomic_dec_and_test(&dest->refcnt)) {
		IP_VS_DBG_BUF(3, "Removing destination %u/%s:%u\n",
			      dest->vfwmark,
			      IP_VS_DBG_ADDR(dest->af, &dest->addr),
			      ntohs(dest->port));
		ip_vs_dst_reset(dest);
		 
		atomic_dec(&dest->svc->refcnt);
		free_percpu(dest->stats.cpustats);
		kfree(dest);
	} else {
		IP_VS_DBG_BUF(3, "Moving dest %s:%u into trash, "
			      "dest->refcnt=%d\n",
			      IP_VS_DBG_ADDR(dest->af, &dest->addr),
			      ntohs(dest->port),
			      atomic_read(&dest->refcnt));
		list_add(&dest->n_list, &ipvs->dest_trash);
		atomic_inc(&dest->refcnt);
	}
}
