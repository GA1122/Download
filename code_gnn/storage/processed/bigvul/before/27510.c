static netdev_tx_t ipgre_tunnel_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	struct netdev_queue *txq = netdev_get_tx_queue(dev, 0);
	struct iphdr  *old_iph = ip_hdr(skb);
	struct iphdr  *tiph;
	u8     tos;
	__be16 df;
	struct rtable *rt;     			 
	struct net_device *tdev;			 
	struct iphdr  *iph;			 
	unsigned int max_headroom;		 
	int    gre_hlen;
	__be32 dst;
	int    mtu;

	if (dev->type == ARPHRD_ETHER)
		IPCB(skb)->flags = 0;

	if (dev->header_ops && dev->type == ARPHRD_IPGRE) {
		gre_hlen = 0;
		tiph = (struct iphdr *)skb->data;
	} else {
		gre_hlen = tunnel->hlen;
		tiph = &tunnel->parms.iph;
	}

	if ((dst = tiph->daddr) == 0) {
		 

		if (skb_dst(skb) == NULL) {
			stats->tx_fifo_errors++;
			goto tx_error;
		}

		if (skb->protocol == htons(ETH_P_IP)) {
			rt = skb_rtable(skb);
			if ((dst = rt->rt_gateway) == 0)
				goto tx_error_icmp;
		}
#ifdef CONFIG_IPV6
		else if (skb->protocol == htons(ETH_P_IPV6)) {
			struct in6_addr *addr6;
			int addr_type;
			struct neighbour *neigh = skb_dst(skb)->neighbour;

			if (neigh == NULL)
				goto tx_error;

			addr6 = (struct in6_addr *)&neigh->primary_key;
			addr_type = ipv6_addr_type(addr6);

			if (addr_type == IPV6_ADDR_ANY) {
				addr6 = &ipv6_hdr(skb)->daddr;
				addr_type = ipv6_addr_type(addr6);
			}

			if ((addr_type & IPV6_ADDR_COMPATv4) == 0)
				goto tx_error_icmp;

			dst = addr6->s6_addr32[3];
		}
#endif
		else
			goto tx_error;
	}

	tos = tiph->tos;
	if (tos == 1) {
		tos = 0;
		if (skb->protocol == htons(ETH_P_IP))
			tos = old_iph->tos;
	}

	{
		struct flowi fl = { .oif = tunnel->parms.link,
				    .nl_u = { .ip4_u =
					      { .daddr = dst,
						.saddr = tiph->saddr,
						.tos = RT_TOS(tos) } },
				    .proto = IPPROTO_GRE };
		if (ip_route_output_key(dev_net(dev), &rt, &fl)) {
			stats->tx_carrier_errors++;
			goto tx_error;
		}
	}
	tdev = rt->u.dst.dev;

	if (tdev == dev) {
		ip_rt_put(rt);
		stats->collisions++;
		goto tx_error;
	}

	df = tiph->frag_off;
	if (df)
		mtu = dst_mtu(&rt->u.dst) - dev->hard_header_len - tunnel->hlen;
	else
		mtu = skb_dst(skb) ? dst_mtu(skb_dst(skb)) : dev->mtu;

	if (skb_dst(skb))
		skb_dst(skb)->ops->update_pmtu(skb_dst(skb), mtu);

	if (skb->protocol == htons(ETH_P_IP)) {
		df |= (old_iph->frag_off&htons(IP_DF));

		if ((old_iph->frag_off&htons(IP_DF)) &&
		    mtu < ntohs(old_iph->tot_len)) {
			icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED, htonl(mtu));
			ip_rt_put(rt);
			goto tx_error;
		}
	}
#ifdef CONFIG_IPV6
	else if (skb->protocol == htons(ETH_P_IPV6)) {
		struct rt6_info *rt6 = (struct rt6_info *)skb_dst(skb);

		if (rt6 && mtu < dst_mtu(skb_dst(skb)) && mtu >= IPV6_MIN_MTU) {
			if ((tunnel->parms.iph.daddr &&
			     !ipv4_is_multicast(tunnel->parms.iph.daddr)) ||
			    rt6->rt6i_dst.plen == 128) {
				rt6->rt6i_flags |= RTF_MODIFIED;
				skb_dst(skb)->metrics[RTAX_MTU-1] = mtu;
			}
		}

		if (mtu >= IPV6_MIN_MTU && mtu < skb->len - tunnel->hlen + gre_hlen) {
			icmpv6_send(skb, ICMPV6_PKT_TOOBIG, 0, mtu, dev);
			ip_rt_put(rt);
			goto tx_error;
		}
	}
#endif

	if (tunnel->err_count > 0) {
		if (time_before(jiffies,
				tunnel->err_time + IPTUNNEL_ERR_TIMEO)) {
			tunnel->err_count--;

			dst_link_failure(skb);
		} else
			tunnel->err_count = 0;
	}

	max_headroom = LL_RESERVED_SPACE(tdev) + gre_hlen;

	if (skb_headroom(skb) < max_headroom || skb_shared(skb)||
	    (skb_cloned(skb) && !skb_clone_writable(skb, 0))) {
		struct sk_buff *new_skb = skb_realloc_headroom(skb, max_headroom);
		if (!new_skb) {
			ip_rt_put(rt);
			txq->tx_dropped++;
			dev_kfree_skb(skb);
			return NETDEV_TX_OK;
		}
		if (skb->sk)
			skb_set_owner_w(new_skb, skb->sk);
		dev_kfree_skb(skb);
		skb = new_skb;
		old_iph = ip_hdr(skb);
	}

	skb_reset_transport_header(skb);
	skb_push(skb, gre_hlen);
	skb_reset_network_header(skb);
	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	IPCB(skb)->flags &= ~(IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED |
			      IPSKB_REROUTED);
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->u.dst);

	 

	iph 			=	ip_hdr(skb);
	iph->version		=	4;
	iph->ihl		=	sizeof(struct iphdr) >> 2;
	iph->frag_off		=	df;
	iph->protocol		=	IPPROTO_GRE;
	iph->tos		=	ipgre_ecn_encapsulate(tos, old_iph, skb);
	iph->daddr		=	rt->rt_dst;
	iph->saddr		=	rt->rt_src;

	if ((iph->ttl = tiph->ttl) == 0) {
		if (skb->protocol == htons(ETH_P_IP))
			iph->ttl = old_iph->ttl;
#ifdef CONFIG_IPV6
		else if (skb->protocol == htons(ETH_P_IPV6))
			iph->ttl = ((struct ipv6hdr *)old_iph)->hop_limit;
#endif
		else
			iph->ttl = dst_metric(&rt->u.dst, RTAX_HOPLIMIT);
	}

	((__be16 *)(iph + 1))[0] = tunnel->parms.o_flags;
	((__be16 *)(iph + 1))[1] = (dev->type == ARPHRD_ETHER) ?
				   htons(ETH_P_TEB) : skb->protocol;

	if (tunnel->parms.o_flags&(GRE_KEY|GRE_CSUM|GRE_SEQ)) {
		__be32 *ptr = (__be32*)(((u8*)iph) + tunnel->hlen - 4);

		if (tunnel->parms.o_flags&GRE_SEQ) {
			++tunnel->o_seqno;
			*ptr = htonl(tunnel->o_seqno);
			ptr--;
		}
		if (tunnel->parms.o_flags&GRE_KEY) {
			*ptr = tunnel->parms.o_key;
			ptr--;
		}
		if (tunnel->parms.o_flags&GRE_CSUM) {
			*ptr = 0;
			*(__sum16*)ptr = ip_compute_csum((void*)(iph+1), skb->len - sizeof(struct iphdr));
		}
	}

	nf_reset(skb);

	IPTUNNEL_XMIT();
	return NETDEV_TX_OK;

tx_error_icmp:
	dst_link_failure(skb);

tx_error:
	stats->tx_errors++;
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}