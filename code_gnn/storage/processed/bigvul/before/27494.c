static int ipgre_rcv(struct sk_buff *skb)
{
	struct iphdr *iph;
	u8     *h;
	__be16    flags;
	__sum16   csum = 0;
	__be32 key = 0;
	u32    seqno = 0;
	struct ip_tunnel *tunnel;
	int    offset = 4;
	__be16 gre_proto;
	unsigned int len;

	if (!pskb_may_pull(skb, 16))
		goto drop_nolock;

	iph = ip_hdr(skb);
	h = skb->data;
	flags = *(__be16*)h;

	if (flags&(GRE_CSUM|GRE_KEY|GRE_ROUTING|GRE_SEQ|GRE_VERSION)) {
		 
		if (flags&(GRE_VERSION|GRE_ROUTING))
			goto drop_nolock;

		if (flags&GRE_CSUM) {
			switch (skb->ip_summed) {
			case CHECKSUM_COMPLETE:
				csum = csum_fold(skb->csum);
				if (!csum)
					break;
				 
			case CHECKSUM_NONE:
				skb->csum = 0;
				csum = __skb_checksum_complete(skb);
				skb->ip_summed = CHECKSUM_COMPLETE;
			}
			offset += 4;
		}
		if (flags&GRE_KEY) {
			key = *(__be32*)(h + offset);
			offset += 4;
		}
		if (flags&GRE_SEQ) {
			seqno = ntohl(*(__be32*)(h + offset));
			offset += 4;
		}
	}

	gre_proto = *(__be16 *)(h + 2);

	rcu_read_lock();
	if ((tunnel = ipgre_tunnel_lookup(skb->dev,
					  iph->saddr, iph->daddr, key,
					  gre_proto))) {
		struct net_device_stats *stats = &tunnel->dev->stats;

		secpath_reset(skb);

		skb->protocol = gre_proto;
		 
		if (flags == 0 && gre_proto == htons(ETH_P_WCCP)) {
			skb->protocol = htons(ETH_P_IP);
			if ((*(h + offset) & 0xF0) != 0x40)
				offset += 4;
		}

		skb->mac_header = skb->network_header;
		__pskb_pull(skb, offset);
		skb_postpull_rcsum(skb, skb_transport_header(skb), offset);
		skb->pkt_type = PACKET_HOST;
#ifdef CONFIG_NET_IPGRE_BROADCAST
		if (ipv4_is_multicast(iph->daddr)) {
			 
			if (skb_rtable(skb)->fl.iif == 0)
				goto drop;
			stats->multicast++;
			skb->pkt_type = PACKET_BROADCAST;
		}
#endif

		if (((flags&GRE_CSUM) && csum) ||
		    (!(flags&GRE_CSUM) && tunnel->parms.i_flags&GRE_CSUM)) {
			stats->rx_crc_errors++;
			stats->rx_errors++;
			goto drop;
		}
		if (tunnel->parms.i_flags&GRE_SEQ) {
			if (!(flags&GRE_SEQ) ||
			    (tunnel->i_seqno && (s32)(seqno - tunnel->i_seqno) < 0)) {
				stats->rx_fifo_errors++;
				stats->rx_errors++;
				goto drop;
			}
			tunnel->i_seqno = seqno + 1;
		}

		len = skb->len;

		 
		if (tunnel->dev->type == ARPHRD_ETHER) {
			if (!pskb_may_pull(skb, ETH_HLEN)) {
				stats->rx_length_errors++;
				stats->rx_errors++;
				goto drop;
			}

			iph = ip_hdr(skb);
			skb->protocol = eth_type_trans(skb, tunnel->dev);
			skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);
		}

		stats->rx_packets++;
		stats->rx_bytes += len;
		skb->dev = tunnel->dev;
		skb_dst_drop(skb);
		nf_reset(skb);

		skb_reset_network_header(skb);
		ipgre_ecn_decapsulate(iph, skb);

		netif_rx(skb);
		rcu_read_unlock();
		return(0);
	}
	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);

drop:
	rcu_read_unlock();
drop_nolock:
	kfree_skb(skb);
	return(0);
}