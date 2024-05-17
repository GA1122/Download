struct net_device *dev_get_by_index_rcu(struct net *net, int ifindex)
{
	struct hlist_node *p;
	struct net_device *dev;
	struct hlist_head *head = dev_index_hash(net, ifindex);

	hlist_for_each_entry_rcu(dev, p, head, index_hlist)
		if (dev->ifindex == ifindex)
			return dev;

	return NULL;
}