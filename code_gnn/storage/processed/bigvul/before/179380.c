 int sctp_rcv(struct sk_buff *skb)
 {
 	struct sock *sk;
 	struct sctp_association *asoc;
 	struct sctp_endpoint *ep = NULL;
 	struct sctp_ep_common *rcvr;
 	struct sctp_transport *transport = NULL;
 	struct sctp_chunk *chunk;
 	struct sctphdr *sh;
 	union sctp_addr src;
 	union sctp_addr dest;
 	int family;
 	struct sctp_af *af;
 
 	if (skb->pkt_type!=PACKET_HOST)
 		goto discard_it;
 
 	SCTP_INC_STATS_BH(SCTP_MIB_INSCTPPACKS);
 
 	if (skb_linearize(skb))
 		goto discard_it;
 
 	sh = sctp_hdr(skb);
 
 	 
 	__skb_pull(skb, skb_transport_offset(skb));
 	if (skb->len < sizeof(struct sctphdr))
 		goto discard_it;
 	if (!skb_csum_unnecessary(skb) && sctp_rcv_checksum(skb) < 0)
 		goto discard_it;
 
 	skb_pull(skb, sizeof(struct sctphdr));
 
 	 
 	if (skb->len < sizeof(struct sctp_chunkhdr))
 		goto discard_it;
 
 	family = ipver2af(ip_hdr(skb)->version);
 	af = sctp_get_af_specific(family);
 	if (unlikely(!af))
 		goto discard_it;
 
 	 
 	af->from_skb(&src, skb, 1);
 	af->from_skb(&dest, skb, 0);
 
 	 
 	if (!af->addr_valid(&src, NULL, skb) ||
 	    !af->addr_valid(&dest, NULL, skb))
 		goto discard_it;
 
 	asoc = __sctp_rcv_lookup(skb, &src, &dest, &transport);
 
 	if (!asoc)
 		ep = __sctp_rcv_lookup_endpoint(&dest);
 
 	 
 	rcvr = asoc ? &asoc->base : &ep->base;
 	sk = rcvr->sk;
 
 	 
 	if (sk->sk_bound_dev_if && (sk->sk_bound_dev_if != af->skb_iif(skb)))
 	{
 		if (asoc) {
 			sctp_association_put(asoc);
 			asoc = NULL;
 		} else {
 			sctp_endpoint_put(ep);
 			ep = NULL;
 		}
 		sk = sctp_get_ctl_sock();
 		ep = sctp_sk(sk)->ep;
 		sctp_endpoint_hold(ep);
 		rcvr = &ep->base;
 	}
 
 	 
 	if (!asoc) {
 		if (sctp_rcv_ootb(skb)) {
 			SCTP_INC_STATS_BH(SCTP_MIB_OUTOFBLUES);
 			goto discard_release;
 		}
 	}
 
 	if (!xfrm_policy_check(sk, XFRM_POLICY_IN, skb, family))
 		goto discard_release;
 	nf_reset(skb);
 
 	if (sk_filter(sk, skb))
 		goto discard_release;
 
 	 
 	chunk = sctp_chunkify(skb, asoc, sk);
 	if (!chunk)
 		goto discard_release;
 	SCTP_INPUT_CB(skb)->chunk = chunk;
 
 	 
 	chunk->rcvr = rcvr;
 
 	 
 	chunk->sctp_hdr = sh;
 
 	 
 	sctp_init_addrs(chunk, &src, &dest);
 
 	 
 	chunk->transport = transport;
 
 	 
  	sctp_bh_lock_sock(sk);
  
// 	if (sk != rcvr->sk) {
// 		 
// 		sctp_bh_unlock_sock(sk);
// 		sk = rcvr->sk;
// 		sctp_bh_lock_sock(sk);
// 	}
// 
  	if (sock_owned_by_user(sk)) {
  		SCTP_INC_STATS_BH(SCTP_MIB_IN_PKT_BACKLOG);
  		sctp_add_backlog(sk, skb);
 	} else {
 		SCTP_INC_STATS_BH(SCTP_MIB_IN_PKT_SOFTIRQ);
 		sctp_inq_push(&chunk->rcvr->inqueue, chunk);
 	}
 
 	sctp_bh_unlock_sock(sk);
 
 	 
 	if (asoc)
 		sctp_association_put(asoc);
 	else
 		sctp_endpoint_put(ep);
 
 	return 0;
 
 discard_it:
 	SCTP_INC_STATS_BH(SCTP_MIB_IN_PKT_DISCARDS);
 	kfree_skb(skb);
 	return 0;
 
 discard_release:
 	 
 	if (asoc)
 		sctp_association_put(asoc);
 	else
 		sctp_endpoint_put(ep);
 
 	goto discard_it;
 }