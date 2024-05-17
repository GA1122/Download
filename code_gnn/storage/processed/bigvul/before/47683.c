void icmpv6_notify(struct sk_buff *skb, u8 type, u8 code, __be32 info)
{
	const struct inet6_protocol *ipprot;
	int inner_offset;
	__be16 frag_off;
	u8 nexthdr;
	struct net *net = dev_net(skb->dev);

	if (!pskb_may_pull(skb, sizeof(struct ipv6hdr)))
		goto out;

	nexthdr = ((struct ipv6hdr *)skb->data)->nexthdr;
	if (ipv6_ext_hdr(nexthdr)) {
		 
		inner_offset = ipv6_skip_exthdr(skb, sizeof(struct ipv6hdr),
						&nexthdr, &frag_off);
		if (inner_offset < 0)
			goto out;
	} else {
		inner_offset = sizeof(struct ipv6hdr);
	}

	 
	if (!pskb_may_pull(skb, inner_offset+8))
		goto out;

	 

	ipprot = rcu_dereference(inet6_protos[nexthdr]);
	if (ipprot && ipprot->err_handler)
		ipprot->err_handler(skb, NULL, type, code, inner_offset, info);

	raw6_icmp_error(skb, nexthdr, type, code, inner_offset, info);
	return;

out:
	__ICMP6_INC_STATS(net, __in6_dev_get(skb->dev), ICMP6_MIB_INERRORS);
}
