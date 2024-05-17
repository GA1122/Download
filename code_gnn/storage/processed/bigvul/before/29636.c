static void sctp_v6_copy_addrlist(struct list_head *addrlist,
				  struct net_device *dev)
{
	struct inet6_dev *in6_dev;
	struct inet6_ifaddr *ifp;
	struct sctp_sockaddr_entry *addr;

	rcu_read_lock();
	if ((in6_dev = __in6_dev_get(dev)) == NULL) {
		rcu_read_unlock();
		return;
	}

	read_lock_bh(&in6_dev->lock);
	list_for_each_entry(ifp, &in6_dev->addr_list, if_list) {
		 
		addr = kzalloc(sizeof(*addr), GFP_ATOMIC);
		if (addr) {
			addr->a.v6.sin6_family = AF_INET6;
			addr->a.v6.sin6_port = 0;
			addr->a.v6.sin6_addr = ifp->addr;
			addr->a.v6.sin6_scope_id = dev->ifindex;
			addr->valid = 1;
			INIT_LIST_HEAD(&addr->list);
			list_add_tail(&addr->list, addrlist);
		}
	}

	read_unlock_bh(&in6_dev->lock);
	rcu_read_unlock();
}