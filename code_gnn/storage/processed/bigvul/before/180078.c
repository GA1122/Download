 static struct sk_buff **gre_gro_receive(struct sk_buff **head,
 					struct sk_buff *skb)
 {
 	struct sk_buff **pp = NULL;
 	struct sk_buff *p;
 	const struct gre_base_hdr *greh;
 	unsigned int hlen, grehlen;
 	unsigned int off;
 	int flush = 1;
  	struct packet_offload *ptype;
  	__be16 type;
  
// 	if (NAPI_GRO_CB(skb)->encap_mark)
// 		goto out;
// 
// 	NAPI_GRO_CB(skb)->encap_mark = 1;
// 
  	off = skb_gro_offset(skb);
  	hlen = off + sizeof(*greh);
  	greh = skb_gro_header_fast(skb, off);
 	if (skb_gro_header_hard(skb, hlen)) {
 		greh = skb_gro_header_slow(skb, hlen, off);
 		if (unlikely(!greh))
 			goto out;
 	}
 
 	 
 	if ((greh->flags & ~(GRE_KEY|GRE_CSUM)) != 0)
 		goto out;
 
 	type = greh->protocol;
 
 	rcu_read_lock();
 	ptype = gro_find_receive_by_type(type);
 	if (!ptype)
 		goto out_unlock;
 
 	grehlen = GRE_HEADER_SECTION;
 
 	if (greh->flags & GRE_KEY)
 		grehlen += GRE_HEADER_SECTION;
 
 	if (greh->flags & GRE_CSUM)
 		grehlen += GRE_HEADER_SECTION;
 
 	hlen = off + grehlen;
 	if (skb_gro_header_hard(skb, hlen)) {
 		greh = skb_gro_header_slow(skb, hlen, off);
 		if (unlikely(!greh))
 			goto out_unlock;
 	}
 
 	 
 	if ((greh->flags & GRE_CSUM) && !NAPI_GRO_CB(skb)->flush) {
 		if (skb_gro_checksum_simple_validate(skb))
 			goto out_unlock;
 
 		skb_gro_checksum_try_convert(skb, IPPROTO_GRE, 0,
 					     null_compute_pseudo);
 	}
 
 	for (p = *head; p; p = p->next) {
 		const struct gre_base_hdr *greh2;
 
 		if (!NAPI_GRO_CB(p)->same_flow)
 			continue;
 
 		 
 		greh2 = (struct gre_base_hdr *)(p->data + off);
 
 		if (greh2->flags != greh->flags ||
 		    greh2->protocol != greh->protocol) {
 			NAPI_GRO_CB(p)->same_flow = 0;
 			continue;
 		}
 		if (greh->flags & GRE_KEY) {
 			 
 			if (*(__be32 *)(greh2+1) != *(__be32 *)(greh+1)) {
 				NAPI_GRO_CB(p)->same_flow = 0;
 				continue;
 			}
 		}
 	}
 
 	skb_gro_pull(skb, grehlen);
 
 	 
 	skb_gro_postpull_rcsum(skb, greh, grehlen);
 
 	pp = ptype->callbacks.gro_receive(head, skb);
 	flush = 0;
 
 out_unlock:
 	rcu_read_unlock();
 out:
 	NAPI_GRO_CB(skb)->flush |= flush;
 
 	return pp;
 }