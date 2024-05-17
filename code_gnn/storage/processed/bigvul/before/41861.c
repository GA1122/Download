int ipv6_chk_addr_and_flags(struct net *net, const struct in6_addr *addr,
			    const struct net_device *dev, int strict,
			    u32 banned_flags)
{
	struct inet6_ifaddr *ifp;
	unsigned int hash = inet6_addr_hash(addr);
	u32 ifp_flags;

	rcu_read_lock_bh();
	hlist_for_each_entry_rcu(ifp, &inet6_addr_lst[hash], addr_lst) {
		if (!net_eq(dev_net(ifp->idev->dev), net))
			continue;
		 
		ifp_flags = (ifp->flags&IFA_F_OPTIMISTIC)
			    ? (ifp->flags&~IFA_F_TENTATIVE)
			    : ifp->flags;
		if (ipv6_addr_equal(&ifp->addr, addr) &&
		    !(ifp_flags&banned_flags) &&
		    (dev == NULL || ifp->idev->dev == dev ||
		     !(ifp->scope&(IFA_LINK|IFA_HOST) || strict))) {
			rcu_read_unlock_bh();
			return 1;
		}
	}

	rcu_read_unlock_bh();
	return 0;
}