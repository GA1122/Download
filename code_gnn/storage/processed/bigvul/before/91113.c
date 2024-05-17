int fib_multipath_hash(const struct net *net, const struct flowi4 *fl4,
		       const struct sk_buff *skb, struct flow_keys *flkeys)
{
	u32 multipath_hash = fl4 ? fl4->flowi4_multipath_hash : 0;
	struct flow_keys hash_keys;
	u32 mhash;

	switch (net->ipv4.sysctl_fib_multipath_hash_policy) {
	case 0:
		memset(&hash_keys, 0, sizeof(hash_keys));
		hash_keys.control.addr_type = FLOW_DISSECTOR_KEY_IPV4_ADDRS;
		if (skb) {
			ip_multipath_l3_keys(skb, &hash_keys);
		} else {
			hash_keys.addrs.v4addrs.src = fl4->saddr;
			hash_keys.addrs.v4addrs.dst = fl4->daddr;
		}
		break;
	case 1:
		 
		if (skb) {
			unsigned int flag = FLOW_DISSECTOR_F_STOP_AT_ENCAP;
			struct flow_keys keys;

			 
			if (skb->l4_hash)
				return skb_get_hash_raw(skb) >> 1;

			memset(&hash_keys, 0, sizeof(hash_keys));

			if (!flkeys) {
				skb_flow_dissect_flow_keys(skb, &keys, flag);
				flkeys = &keys;
			}

			hash_keys.control.addr_type = FLOW_DISSECTOR_KEY_IPV4_ADDRS;
			hash_keys.addrs.v4addrs.src = flkeys->addrs.v4addrs.src;
			hash_keys.addrs.v4addrs.dst = flkeys->addrs.v4addrs.dst;
			hash_keys.ports.src = flkeys->ports.src;
			hash_keys.ports.dst = flkeys->ports.dst;
			hash_keys.basic.ip_proto = flkeys->basic.ip_proto;
		} else {
			memset(&hash_keys, 0, sizeof(hash_keys));
			hash_keys.control.addr_type = FLOW_DISSECTOR_KEY_IPV4_ADDRS;
			hash_keys.addrs.v4addrs.src = fl4->saddr;
			hash_keys.addrs.v4addrs.dst = fl4->daddr;
			hash_keys.ports.src = fl4->fl4_sport;
			hash_keys.ports.dst = fl4->fl4_dport;
			hash_keys.basic.ip_proto = fl4->flowi4_proto;
		}
		break;
	}
	mhash = flow_hash_from_keys(&hash_keys);

	if (multipath_hash)
		mhash = jhash_2words(mhash, multipath_hash, 0);

	return mhash >> 1;
}
