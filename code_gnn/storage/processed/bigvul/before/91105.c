static void unhash_nsid(struct net *net, struct net *last)
{
	struct net *tmp;
	 
	for_each_net(tmp) {
		int id;

		spin_lock_bh(&tmp->nsid_lock);
		id = __peernet2id(tmp, net);
		if (id >= 0)
			idr_remove(&tmp->netns_ids, id);
		spin_unlock_bh(&tmp->nsid_lock);
		if (id >= 0)
			rtnl_net_notifyid(tmp, RTM_DELNSID, id);
		if (tmp == last)
			break;
	}
	spin_lock_bh(&net->nsid_lock);
	idr_destroy(&net->netns_ids);
	spin_unlock_bh(&net->nsid_lock);
}
