 static int ip6_frag_queue(struct frag_queue *fq, struct sk_buff *skb,
 			   struct frag_hdr *fhdr, int nhoff)
 {
 	struct sk_buff *prev, *next;
 	struct net_device *dev;
 	int offset, end;
 	struct net *net = dev_net(skb_dst(skb)->dev);
 
 	if (fq->q.last_in & INET_FRAG_COMPLETE)
 		goto err;
 
 	offset = ntohs(fhdr->frag_off) & ~0x7;
 	end = offset + (ntohs(ipv6_hdr(skb)->payload_len) -
 			((u8 *)(fhdr + 1) - (u8 *)(ipv6_hdr(skb) + 1)));
 
 	if ((unsigned int)end > IPV6_MAXPLEN) {
 		IP6_INC_STATS_BH(net, ip6_dst_idev(skb_dst(skb)),
 				 IPSTATS_MIB_INHDRERRORS);
 		icmpv6_param_prob(skb, ICMPV6_HDR_FIELD,
 				  ((u8 *)&fhdr->frag_off -
 				   skb_network_header(skb)));
 		return -1;
 	}
 
 	if (skb->ip_summed == CHECKSUM_COMPLETE) {
 		const unsigned char *nh = skb_network_header(skb);
 		skb->csum = csum_sub(skb->csum,
 				     csum_partial(nh, (u8 *)(fhdr + 1) - nh,
 						  0));
 	}
 
 	 
 	if (!(fhdr->frag_off & htons(IP6_MF))) {
 		 
 		if (end < fq->q.len ||
 		    ((fq->q.last_in & INET_FRAG_LAST_IN) && end != fq->q.len))
 			goto err;
 		fq->q.last_in |= INET_FRAG_LAST_IN;
 		fq->q.len = end;
 	} else {
 		 
 		if (end & 0x7) {
 			 
 			IP6_INC_STATS_BH(net, ip6_dst_idev(skb_dst(skb)),
 					 IPSTATS_MIB_INHDRERRORS);
 			icmpv6_param_prob(skb, ICMPV6_HDR_FIELD,
 					  offsetof(struct ipv6hdr, payload_len));
 			return -1;
 		}
 		if (end > fq->q.len) {
 			 
 			if (fq->q.last_in & INET_FRAG_LAST_IN)
 				goto err;
 			fq->q.len = end;
 		}
 	}
 
 	if (end == offset)
 		goto err;
 
 	 
 	if (!pskb_pull(skb, (u8 *) (fhdr + 1) - skb->data))
 		goto err;
 
 	if (pskb_trim_rcsum(skb, end - offset))
 		goto err;
 
 	 
 	prev = fq->q.fragments_tail;
 	if (!prev || FRAG6_CB(prev)->offset < offset) {
 		next = NULL;
 		goto found;
 	}
 	prev = NULL;
 	for(next = fq->q.fragments; next != NULL; next = next->next) {
 		if (FRAG6_CB(next)->offset >= offset)
 			break;	 
 		prev = next;
  	}
  
  found:
 	 
	if (prev) {
		int i = (FRAG6_CB(prev)->offset + prev->len) - offset;
  
		if (i > 0) {
			offset += i;
			if (end <= offset)
				goto err;
			if (!pskb_pull(skb, i))
				goto err;
			if (skb->ip_summed != CHECKSUM_UNNECESSARY)
				skb->ip_summed = CHECKSUM_NONE;
		}
	}
// 	 
// 	if (prev &&
// 	    (FRAG6_CB(prev)->offset + prev->len) - offset > 0)
// 		goto discard_fq;
  
 	 
	while (next && FRAG6_CB(next)->offset < end) {
		int i = end - FRAG6_CB(next)->offset;  
		if (i < next->len) {
			 
			if (!pskb_pull(next, i))
				goto err;
			FRAG6_CB(next)->offset += i;	 
			fq->q.meat -= i;
			if (next->ip_summed != CHECKSUM_UNNECESSARY)
				next->ip_summed = CHECKSUM_NONE;
			break;
		} else {
			struct sk_buff *free_it = next;
			 
			next = next->next;
			if (prev)
				prev->next = next;
			else
				fq->q.fragments = next;
			fq->q.meat -= free_it->len;
			frag_kfree_skb(fq->q.net, free_it);
		}
	}
// 	if (next && FRAG6_CB(next)->offset < end)
// 		goto discard_fq;
  
  	FRAG6_CB(skb)->offset = offset;
  
 	 
 	skb->next = next;
 	if (!next)
 		fq->q.fragments_tail = skb;
 	if (prev)
 		prev->next = skb;
 	else
 		fq->q.fragments = skb;
 
 	dev = skb->dev;
 	if (dev) {
 		fq->iif = dev->ifindex;
 		skb->dev = NULL;
 	}
 	fq->q.stamp = skb->tstamp;
 	fq->q.meat += skb->len;
 	atomic_add(skb->truesize, &fq->q.net->mem);
 
 	 
 	if (offset == 0) {
 		fq->nhoffset = nhoff;
 		fq->q.last_in |= INET_FRAG_FIRST_IN;
 	}
 
 	if (fq->q.last_in == (INET_FRAG_FIRST_IN | INET_FRAG_LAST_IN) &&
 	    fq->q.meat == fq->q.len)
 		return ip6_frag_reasm(fq, prev, dev);
 
 	write_lock(&ip6_frags.lock);
 	list_move_tail(&fq->q.lru_list, &fq->q.net->lru_list);
  	write_unlock(&ip6_frags.lock);
  	return -1;
  
// discard_fq:
// 	fq_kill(fq);
  err:
  	IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
  		      IPSTATS_MIB_REASMFAILS);
 	kfree_skb(skb);
 	return -1;
 }