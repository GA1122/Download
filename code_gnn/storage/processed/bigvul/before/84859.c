ebt_basic_match(const struct ebt_entry *e, const struct sk_buff *skb,
		const struct net_device *in, const struct net_device *out)
{
	const struct ethhdr *h = eth_hdr(skb);
	const struct net_bridge_port *p;
	__be16 ethproto;

	if (skb_vlan_tag_present(skb))
		ethproto = htons(ETH_P_8021Q);
	else
		ethproto = h->h_proto;

	if (e->bitmask & EBT_802_3) {
		if (NF_INVF(e, EBT_IPROTO, eth_proto_is_802_3(ethproto)))
			return 1;
	} else if (!(e->bitmask & EBT_NOPROTO) &&
		   NF_INVF(e, EBT_IPROTO, e->ethproto != ethproto))
		return 1;

	if (NF_INVF(e, EBT_IIN, ebt_dev_check(e->in, in)))
		return 1;
	if (NF_INVF(e, EBT_IOUT, ebt_dev_check(e->out, out)))
		return 1;
	 
	if (in && (p = br_port_get_rcu(in)) != NULL &&
	    NF_INVF(e, EBT_ILOGICALIN,
		    ebt_dev_check(e->logical_in, p->br->dev)))
		return 1;
	if (out && (p = br_port_get_rcu(out)) != NULL &&
	    NF_INVF(e, EBT_ILOGICALOUT,
		    ebt_dev_check(e->logical_out, p->br->dev)))
		return 1;

	if (e->bitmask & EBT_SOURCEMAC) {
		if (NF_INVF(e, EBT_ISOURCE,
			    !ether_addr_equal_masked(h->h_source, e->sourcemac,
						     e->sourcemsk)))
			return 1;
	}
	if (e->bitmask & EBT_DESTMAC) {
		if (NF_INVF(e, EBT_IDEST,
			    !ether_addr_equal_masked(h->h_dest, e->destmac,
						     e->destmsk)))
			return 1;
	}
	return 0;
}