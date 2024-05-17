void ndisc_send_rs(struct net_device *dev, const struct in6_addr *saddr,
		   const struct in6_addr *daddr)
{
	struct sk_buff *skb;
	struct rs_msg *msg;
	int send_sllao = dev->addr_len;
	int optlen = 0;

#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	 
	if (send_sllao) {
		struct inet6_ifaddr *ifp = ipv6_get_ifaddr(dev_net(dev), saddr,
							   dev, 1);
		if (ifp) {
			if (ifp->flags & IFA_F_OPTIMISTIC)  {
				send_sllao = 0;
			}
			in6_ifa_put(ifp);
		} else {
			send_sllao = 0;
		}
	}
#endif
	if (send_sllao)
		optlen += ndisc_opt_addr_space(dev);

	skb = ndisc_alloc_skb(dev, sizeof(*msg) + optlen);
	if (!skb)
		return;

	msg = (struct rs_msg *)skb_put(skb, sizeof(*msg));
	*msg = (struct rs_msg) {
		.icmph = {
			.icmp6_type = NDISC_ROUTER_SOLICITATION,
		},
	};

	if (send_sllao)
		ndisc_fill_addr_option(skb, ND_OPT_SOURCE_LL_ADDR,
				       dev->dev_addr);

	ndisc_send_skb(skb, daddr, saddr);
}
