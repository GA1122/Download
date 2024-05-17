inet6_rtm_deladdr(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	struct net *net = sock_net(skb->sk);
	struct ifaddrmsg *ifm;
	struct nlattr *tb[IFA_MAX+1];
	struct in6_addr *pfx, *peer_pfx;
	u32 ifa_flags;
	int err;

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFA_MAX, ifa_ipv6_policy);
	if (err < 0)
		return err;

	ifm = nlmsg_data(nlh);
	pfx = extract_addr(tb[IFA_ADDRESS], tb[IFA_LOCAL], &peer_pfx);
	if (pfx == NULL)
		return -EINVAL;

	ifa_flags = tb[IFA_FLAGS] ? nla_get_u32(tb[IFA_FLAGS]) : ifm->ifa_flags;

	 
	ifa_flags &= IFA_F_MANAGETEMPADDR;

	return inet6_addr_del(net, ifm->ifa_index, ifa_flags, pfx,
			      ifm->ifa_prefixlen);
}