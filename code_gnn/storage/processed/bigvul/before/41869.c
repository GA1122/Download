int ipv6_dev_get_saddr(struct net *net, const struct net_device *dst_dev,
		       const struct in6_addr *daddr, unsigned int prefs,
		       struct in6_addr *saddr)
{
	struct ipv6_saddr_score scores[2],
				*score = &scores[0], *hiscore = &scores[1];
	struct ipv6_saddr_dst dst;
	struct net_device *dev;
	int dst_type;

	dst_type = __ipv6_addr_type(daddr);
	dst.addr = daddr;
	dst.ifindex = dst_dev ? dst_dev->ifindex : 0;
	dst.scope = __ipv6_addr_src_scope(dst_type);
	dst.label = ipv6_addr_label(net, daddr, dst_type, dst.ifindex);
	dst.prefs = prefs;

	hiscore->rule = -1;
	hiscore->ifa = NULL;

	rcu_read_lock();

	for_each_netdev_rcu(net, dev) {
		struct inet6_dev *idev;

		 
		if (((dst_type & IPV6_ADDR_MULTICAST) ||
		     dst.scope <= IPV6_ADDR_SCOPE_LINKLOCAL) &&
		    dst.ifindex && dev->ifindex != dst.ifindex)
			continue;

		idev = __in6_dev_get(dev);
		if (!idev)
			continue;

		read_lock_bh(&idev->lock);
		list_for_each_entry(score->ifa, &idev->addr_list, if_list) {
			int i;

			 
			if ((score->ifa->flags & IFA_F_TENTATIVE) &&
			    (!(score->ifa->flags & IFA_F_OPTIMISTIC)))
				continue;

			score->addr_type = __ipv6_addr_type(&score->ifa->addr);

			if (unlikely(score->addr_type == IPV6_ADDR_ANY ||
				     score->addr_type & IPV6_ADDR_MULTICAST)) {
				net_dbg_ratelimited("ADDRCONF: unspecified / multicast address assigned as unicast address on %s",
						    dev->name);
				continue;
			}

			score->rule = -1;
			bitmap_zero(score->scorebits, IPV6_SADDR_RULE_MAX);

			for (i = 0; i < IPV6_SADDR_RULE_MAX; i++) {
				int minihiscore, miniscore;

				minihiscore = ipv6_get_saddr_eval(net, hiscore, &dst, i);
				miniscore = ipv6_get_saddr_eval(net, score, &dst, i);

				if (minihiscore > miniscore) {
					if (i == IPV6_SADDR_RULE_SCOPE &&
					    score->scopedist > 0) {
						 
						goto try_nextdev;
					}
					break;
				} else if (minihiscore < miniscore) {
					if (hiscore->ifa)
						in6_ifa_put(hiscore->ifa);

					in6_ifa_hold(score->ifa);

					swap(hiscore, score);

					 
					score->ifa = hiscore->ifa;

					break;
				}
			}
		}
try_nextdev:
		read_unlock_bh(&idev->lock);
	}
	rcu_read_unlock();

	if (!hiscore->ifa)
		return -EADDRNOTAVAIL;

	*saddr = hiscore->ifa->addr;
	in6_ifa_put(hiscore->ifa);
	return 0;
}