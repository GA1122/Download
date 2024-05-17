int peernet2id_alloc(struct net *net, struct net *peer)
{
	bool alloc = false, alive = false;
	int id;

	if (refcount_read(&net->count) == 0)
		return NETNSA_NSID_NOT_ASSIGNED;
	spin_lock_bh(&net->nsid_lock);
	 
	if (maybe_get_net(peer))
		alive = alloc = true;
	id = __peernet2id_alloc(net, peer, &alloc);
	spin_unlock_bh(&net->nsid_lock);
	if (alloc && id >= 0)
		rtnl_net_notifyid(net, RTM_NEWNSID, id);
	if (alive)
		put_net(peer);
	return id;
}
