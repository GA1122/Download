static struct sk_buff **inet_gro_receive(struct sk_buff **head,
					 struct sk_buff *skb)
{
	const struct net_offload *ops;
	struct sk_buff **pp = NULL;
	struct sk_buff *p;
	const struct iphdr *iph;
	unsigned int hlen;
	unsigned int off;
	unsigned int id;
	int flush = 1;
	int proto;

	off = skb_gro_offset(skb);
	hlen = off + sizeof(*iph);
	iph = skb_gro_header_fast(skb, off);
	if (skb_gro_header_hard(skb, hlen)) {
		iph = skb_gro_header_slow(skb, hlen, off);
		if (unlikely(!iph))
			goto out;
	}

	proto = iph->protocol;

	rcu_read_lock();
	ops = rcu_dereference(inet_offloads[proto]);
	if (!ops || !ops->callbacks.gro_receive)
		goto out_unlock;

	if (*(u8 *)iph != 0x45)
		goto out_unlock;

	if (unlikely(ip_fast_csum((u8 *)iph, 5)))
		goto out_unlock;

	id = ntohl(*(__be32 *)&iph->id);
	flush = (u16)((ntohl(*(__be32 *)iph) ^ skb_gro_len(skb)) | (id & ~IP_DF));
	id >>= 16;

	for (p = *head; p; p = p->next) {
		struct iphdr *iph2;

		if (!NAPI_GRO_CB(p)->same_flow)
			continue;

		iph2 = (struct iphdr *)(p->data + off);
		 
		if ((iph->protocol ^ iph2->protocol) |
		    ((__force u32)iph->saddr ^ (__force u32)iph2->saddr) |
		    ((__force u32)iph->daddr ^ (__force u32)iph2->daddr)) {
			NAPI_GRO_CB(p)->same_flow = 0;
			continue;
		}

		 
		NAPI_GRO_CB(p)->flush |=
			(iph->ttl ^ iph2->ttl) |
			(iph->tos ^ iph2->tos) |
			((iph->frag_off ^ iph2->frag_off) & htons(IP_DF));

		 
		NAPI_GRO_CB(p)->flush_id =
			    ((u16)(ntohs(iph2->id) + NAPI_GRO_CB(p)->count) ^ id);
		NAPI_GRO_CB(p)->flush |= flush;
	}

	NAPI_GRO_CB(skb)->flush |= flush;
	skb_set_network_header(skb, off);
	 

	 
	skb_gro_pull(skb, sizeof(*iph));
	skb_set_transport_header(skb, skb_gro_offset(skb));

	pp = ops->callbacks.gro_receive(head, skb);

out_unlock:
	rcu_read_unlock();

out:
	NAPI_GRO_CB(skb)->flush |= flush;

	return pp;
}