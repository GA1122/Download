int ip6mr_get_route(struct net *net, struct sk_buff *skb, struct rtmsg *rtm,
		    u32 portid)
{
	int err;
	struct mr6_table *mrt;
	struct mfc6_cache *cache;
	struct rt6_info *rt = (struct rt6_info *)skb_dst(skb);

	mrt = ip6mr_get_table(net, RT6_TABLE_DFLT);
	if (!mrt)
		return -ENOENT;

	read_lock(&mrt_lock);
	cache = ip6mr_cache_find(mrt, &rt->rt6i_src.addr, &rt->rt6i_dst.addr);
	if (!cache && skb->dev) {
		int vif = ip6mr_find_vif(mrt, skb->dev);

		if (vif >= 0)
			cache = ip6mr_cache_find_any(mrt, &rt->rt6i_dst.addr,
						     vif);
	}

	if (!cache) {
		struct sk_buff *skb2;
		struct ipv6hdr *iph;
		struct net_device *dev;
		int vif;

		dev = skb->dev;
		if (!dev || (vif = ip6mr_find_vif(mrt, dev)) < 0) {
			read_unlock(&mrt_lock);
			return -ENODEV;
		}

		 
		skb2 = alloc_skb(sizeof(struct ipv6hdr), GFP_ATOMIC);
		if (!skb2) {
			read_unlock(&mrt_lock);
			return -ENOMEM;
		}

		NETLINK_CB(skb2).portid = portid;
		skb_reset_transport_header(skb2);

		skb_put(skb2, sizeof(struct ipv6hdr));
		skb_reset_network_header(skb2);

		iph = ipv6_hdr(skb2);
		iph->version = 0;
		iph->priority = 0;
		iph->flow_lbl[0] = 0;
		iph->flow_lbl[1] = 0;
		iph->flow_lbl[2] = 0;
		iph->payload_len = 0;
		iph->nexthdr = IPPROTO_NONE;
		iph->hop_limit = 0;
		iph->saddr = rt->rt6i_src.addr;
		iph->daddr = rt->rt6i_dst.addr;

		err = ip6mr_cache_unresolved(mrt, vif, skb2);
		read_unlock(&mrt_lock);

		return err;
	}

	if (rtm->rtm_flags & RTM_F_NOTIFY)
		cache->mfc_flags |= MFC_NOTIFY;

	err = __ip6mr_fill_mroute(mrt, skb, cache, rtm);
	read_unlock(&mrt_lock);
	return err;
}