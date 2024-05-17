static struct in_ifaddr *rtm_to_ifaddr(struct net *net, struct nlmsghdr *nlh,
				       __u32 *pvalid_lft, __u32 *pprefered_lft)
{
	struct nlattr *tb[IFA_MAX+1];
	struct in_ifaddr *ifa;
	struct ifaddrmsg *ifm;
	struct net_device *dev;
	struct in_device *in_dev;
	int err;

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFA_MAX, ifa_ipv4_policy);
	if (err < 0)
		goto errout;

	ifm = nlmsg_data(nlh);
	err = -EINVAL;
	if (ifm->ifa_prefixlen > 32 || !tb[IFA_LOCAL])
		goto errout;

	dev = __dev_get_by_index(net, ifm->ifa_index);
	err = -ENODEV;
	if (!dev)
		goto errout;

	in_dev = __in_dev_get_rtnl(dev);
	err = -ENOBUFS;
	if (!in_dev)
		goto errout;

	ifa = inet_alloc_ifa();
	if (!ifa)
		 
		goto errout;

	ipv4_devconf_setall(in_dev);
	neigh_parms_data_state_setall(in_dev->arp_parms);
	in_dev_hold(in_dev);

	if (!tb[IFA_ADDRESS])
		tb[IFA_ADDRESS] = tb[IFA_LOCAL];

	INIT_HLIST_NODE(&ifa->hash);
	ifa->ifa_prefixlen = ifm->ifa_prefixlen;
	ifa->ifa_mask = inet_make_mask(ifm->ifa_prefixlen);
	ifa->ifa_flags = tb[IFA_FLAGS] ? nla_get_u32(tb[IFA_FLAGS]) :
					 ifm->ifa_flags;
	ifa->ifa_scope = ifm->ifa_scope;
	ifa->ifa_dev = in_dev;

	ifa->ifa_local = nla_get_in_addr(tb[IFA_LOCAL]);
	ifa->ifa_address = nla_get_in_addr(tb[IFA_ADDRESS]);

	if (tb[IFA_BROADCAST])
		ifa->ifa_broadcast = nla_get_in_addr(tb[IFA_BROADCAST]);

	if (tb[IFA_LABEL])
		nla_strlcpy(ifa->ifa_label, tb[IFA_LABEL], IFNAMSIZ);
	else
		memcpy(ifa->ifa_label, dev->name, IFNAMSIZ);

	if (tb[IFA_CACHEINFO]) {
		struct ifa_cacheinfo *ci;

		ci = nla_data(tb[IFA_CACHEINFO]);
		if (!ci->ifa_valid || ci->ifa_prefered > ci->ifa_valid) {
			err = -EINVAL;
			goto errout_free;
		}
		*pvalid_lft = ci->ifa_valid;
		*pprefered_lft = ci->ifa_prefered;
	}

	return ifa;

errout_free:
	inet_free_ifa(ifa);
errout:
	return ERR_PTR(err);
}