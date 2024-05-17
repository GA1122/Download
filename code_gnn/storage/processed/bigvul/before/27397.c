ip4ip6_tnl_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ip6_tnl *t = netdev_priv(dev);
	struct iphdr  *iph = ip_hdr(skb);
	int encap_limit = -1;
	struct flowi fl;
	__u8 dsfield;
	__u32 mtu;
	int err;

	if ((t->parms.proto != IPPROTO_IPIP && t->parms.proto != 0) ||
	    !ip6_tnl_xmit_ctl(t))
		return -1;

	if (!(t->parms.flags & IP6_TNL_F_IGN_ENCAP_LIMIT))
		encap_limit = t->parms.encap_limit;

	memcpy(&fl, &t->fl, sizeof (fl));
	fl.proto = IPPROTO_IPIP;

	dsfield = ipv4_get_dsfield(iph);

	if ((t->parms.flags & IP6_TNL_F_USE_ORIG_TCLASS))
		fl.fl6_flowlabel |= htonl((__u32)iph->tos << IPV6_TCLASS_SHIFT)
					  & IPV6_TCLASS_MASK;

	err = ip6_tnl_xmit2(skb, dev, dsfield, &fl, encap_limit, &mtu);
	if (err != 0) {
		 
		if (err == -EMSGSIZE)
			icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED,
				  htonl(mtu));
		return -1;
	}

	return 0;
}