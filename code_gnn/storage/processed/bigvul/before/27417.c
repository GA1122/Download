static void ip6_tnl_link_config(struct ip6_tnl *t)
{
	struct net_device *dev = t->dev;
	struct ip6_tnl_parm *p = &t->parms;
	struct flowi *fl = &t->fl;

	memcpy(dev->dev_addr, &p->laddr, sizeof(struct in6_addr));
	memcpy(dev->broadcast, &p->raddr, sizeof(struct in6_addr));

	 
	ipv6_addr_copy(&fl->fl6_src, &p->laddr);
	ipv6_addr_copy(&fl->fl6_dst, &p->raddr);
	fl->oif = p->link;
	fl->fl6_flowlabel = 0;

	if (!(p->flags&IP6_TNL_F_USE_ORIG_TCLASS))
		fl->fl6_flowlabel |= IPV6_TCLASS_MASK & p->flowinfo;
	if (!(p->flags&IP6_TNL_F_USE_ORIG_FLOWLABEL))
		fl->fl6_flowlabel |= IPV6_FLOWLABEL_MASK & p->flowinfo;

	ip6_tnl_set_cap(t);

	if (p->flags&IP6_TNL_F_CAP_XMIT && p->flags&IP6_TNL_F_CAP_RCV)
		dev->flags |= IFF_POINTOPOINT;
	else
		dev->flags &= ~IFF_POINTOPOINT;

	dev->iflink = p->link;

	if (p->flags & IP6_TNL_F_CAP_XMIT) {
		int strict = (ipv6_addr_type(&p->raddr) &
			      (IPV6_ADDR_MULTICAST|IPV6_ADDR_LINKLOCAL));

		struct rt6_info *rt = rt6_lookup(dev_net(dev),
						 &p->raddr, &p->laddr,
						 p->link, strict);

		if (rt == NULL)
			return;

		if (rt->rt6i_dev) {
			dev->hard_header_len = rt->rt6i_dev->hard_header_len +
				sizeof (struct ipv6hdr);

			dev->mtu = rt->rt6i_dev->mtu - sizeof (struct ipv6hdr);

			if (dev->mtu < IPV6_MIN_MTU)
				dev->mtu = IPV6_MIN_MTU;
		}
		dst_release(&rt->u.dst);
	}
}