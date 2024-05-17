static int get_rps_cpu(struct net_device *dev, struct sk_buff *skb,
		       struct rps_dev_flow **rflowp)
{
	struct ipv6hdr *ip6;
	struct iphdr *ip;
	struct netdev_rx_queue *rxqueue;
	struct rps_map *map;
	struct rps_dev_flow_table *flow_table;
	struct rps_sock_flow_table *sock_flow_table;
	int cpu = -1;
	u8 ip_proto;
	u16 tcpu;
	u32 addr1, addr2, ihl;
	union {
		u32 v32;
		u16 v16[2];
	} ports;

	if (skb_rx_queue_recorded(skb)) {
		u16 index = skb_get_rx_queue(skb);
		if (unlikely(index >= dev->num_rx_queues)) {
			WARN_ONCE(dev->num_rx_queues > 1, "%s received packet "
				"on queue %u, but number of RX queues is %u\n",
				dev->name, index, dev->num_rx_queues);
			goto done;
		}
		rxqueue = dev->_rx + index;
	} else
		rxqueue = dev->_rx;

	if (!rxqueue->rps_map && !rxqueue->rps_flow_table)
		goto done;

	if (skb->rxhash)
		goto got_hash;  

	switch (skb->protocol) {
	case __constant_htons(ETH_P_IP):
		if (!pskb_may_pull(skb, sizeof(*ip)))
			goto done;

		ip = (struct iphdr *) skb->data;
		ip_proto = ip->protocol;
		addr1 = (__force u32) ip->saddr;
		addr2 = (__force u32) ip->daddr;
		ihl = ip->ihl;
		break;
	case __constant_htons(ETH_P_IPV6):
		if (!pskb_may_pull(skb, sizeof(*ip6)))
			goto done;

		ip6 = (struct ipv6hdr *) skb->data;
		ip_proto = ip6->nexthdr;
		addr1 = (__force u32) ip6->saddr.s6_addr32[3];
		addr2 = (__force u32) ip6->daddr.s6_addr32[3];
		ihl = (40 >> 2);
		break;
	default:
		goto done;
	}
	switch (ip_proto) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
	case IPPROTO_DCCP:
	case IPPROTO_ESP:
	case IPPROTO_AH:
	case IPPROTO_SCTP:
	case IPPROTO_UDPLITE:
		if (pskb_may_pull(skb, (ihl * 4) + 4)) {
			ports.v32 = * (__force u32 *) (skb->data + (ihl * 4));
			if (ports.v16[1] < ports.v16[0])
				swap(ports.v16[0], ports.v16[1]);
			break;
		}
	default:
		ports.v32 = 0;
		break;
	}

	 
	if (addr2 < addr1)
		swap(addr1, addr2);
	skb->rxhash = jhash_3words(addr1, addr2, ports.v32, hashrnd);
	if (!skb->rxhash)
		skb->rxhash = 1;

got_hash:
	flow_table = rcu_dereference(rxqueue->rps_flow_table);
	sock_flow_table = rcu_dereference(rps_sock_flow_table);
	if (flow_table && sock_flow_table) {
		u16 next_cpu;
		struct rps_dev_flow *rflow;

		rflow = &flow_table->flows[skb->rxhash & flow_table->mask];
		tcpu = rflow->cpu;

		next_cpu = sock_flow_table->ents[skb->rxhash &
		    sock_flow_table->mask];

		 
		if (unlikely(tcpu != next_cpu) &&
		    (tcpu == RPS_NO_CPU || !cpu_online(tcpu) ||
		     ((int)(per_cpu(softnet_data, tcpu).input_queue_head -
		      rflow->last_qtail)) >= 0)) {
			tcpu = rflow->cpu = next_cpu;
			if (tcpu != RPS_NO_CPU)
				rflow->last_qtail = per_cpu(softnet_data,
				    tcpu).input_queue_head;
		}
		if (tcpu != RPS_NO_CPU && cpu_online(tcpu)) {
			*rflowp = rflow;
			cpu = tcpu;
			goto done;
		}
	}

	map = rcu_dereference(rxqueue->rps_map);
	if (map) {
		tcpu = map->cpus[((u64) skb->rxhash * map->len) >> 32];

		if (cpu_online(tcpu)) {
			cpu = tcpu;
			goto done;
		}
	}

done:
	return cpu;
}