void inet6_ifa_finish_destroy(struct inet6_ifaddr *ifp)
{
	WARN_ON(!hlist_unhashed(&ifp->addr_lst));

#ifdef NET_REFCNT_DEBUG
	pr_debug("%s\n", __func__);
#endif

	in6_dev_put(ifp->idev);

	if (cancel_delayed_work(&ifp->dad_work))
		pr_notice("delayed DAD work was pending while freeing ifa=%p\n",
			  ifp);

	if (ifp->state != INET6_IFADDR_STATE_DEAD) {
		pr_warn("Freeing alive inet6 address %p\n", ifp);
		return;
	}
	ip6_rt_put(ifp->rt);

	kfree_rcu(ifp, rcu);
}