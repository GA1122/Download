static inline int ip6_ufo_append_data(struct sock *sk,
			struct sk_buff_head *queue,
			int getfrag(void *from, char *to, int offset, int len,
			int odd, struct sk_buff *skb),
			void *from, int length, int hh_len, int fragheaderlen,
			int exthdrlen, int transhdrlen, int mtu,
			unsigned int flags, const struct flowi6 *fl6)

{
	struct sk_buff *skb;
	int err;

	 
	skb = skb_peek_tail(queue);
	if (!skb) {
		skb = sock_alloc_send_skb(sk,
			hh_len + fragheaderlen + transhdrlen + 20,
			(flags & MSG_DONTWAIT), &err);
		if (!skb)
			return err;

		 
		skb_reserve(skb, hh_len);

		 
		skb_put(skb, fragheaderlen + transhdrlen);

		 
		skb_set_network_header(skb, exthdrlen);

		 
		skb->transport_header = skb->network_header + fragheaderlen;

		skb->protocol = htons(ETH_P_IPV6);
		skb->csum = 0;

		if (flags & MSG_CONFIRM)
			skb_set_dst_pending_confirm(skb, 1);

		__skb_queue_tail(queue, skb);
	} else if (skb_is_gso(skb)) {
		goto append;
	}

	skb->ip_summed = CHECKSUM_PARTIAL;
	 
	skb_shinfo(skb)->gso_size = (mtu - fragheaderlen -
				     sizeof(struct frag_hdr)) & ~7;
	skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
	skb_shinfo(skb)->ip6_frag_id = ipv6_select_ident(sock_net(sk),
							 &fl6->daddr,
							 &fl6->saddr);

append:
	return skb_append_datato_frags(sk, skb, getfrag, from,
				       (length - transhdrlen));
}
