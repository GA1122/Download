 static int tcp_v6_do_rcv(struct sock *sk, struct sk_buff *skb)
 {
 	struct ipv6_pinfo *np = inet6_sk(sk);
 	struct tcp_sock *tp;
 	struct sk_buff *opt_skb = NULL;
 
 	 
 
  	if (skb->protocol == htons(ETH_P_IP))
  		return tcp_v4_do_rcv(sk, skb);
  
	if (sk_filter(sk, skb))
// 	if (tcp_filter(sk, skb))
  		goto discard;
  
  	 
 
 	 
 	if (np->rxopt.all)
 		opt_skb = skb_clone(skb, sk_gfp_mask(sk, GFP_ATOMIC));
 
 	if (sk->sk_state == TCP_ESTABLISHED) {  
 		struct dst_entry *dst = sk->sk_rx_dst;
 
 		sock_rps_save_rxhash(sk, skb);
 		sk_mark_napi_id(sk, skb);
 		if (dst) {
 			if (inet_sk(sk)->rx_dst_ifindex != skb->skb_iif ||
 			    dst->ops->check(dst, np->rx_dst_cookie) == NULL) {
 				dst_release(dst);
 				sk->sk_rx_dst = NULL;
 			}
 		}
 
 		tcp_rcv_established(sk, skb, tcp_hdr(skb), skb->len);
 		if (opt_skb)
 			goto ipv6_pktoptions;
 		return 0;
 	}
 
 	if (tcp_checksum_complete(skb))
 		goto csum_err;
 
 	if (sk->sk_state == TCP_LISTEN) {
 		struct sock *nsk = tcp_v6_cookie_check(sk, skb);
 
 		if (!nsk)
 			goto discard;
 
 		if (nsk != sk) {
 			sock_rps_save_rxhash(nsk, skb);
 			sk_mark_napi_id(nsk, skb);
 			if (tcp_child_process(sk, nsk, skb))
 				goto reset;
 			if (opt_skb)
 				__kfree_skb(opt_skb);
 			return 0;
 		}
 	} else
 		sock_rps_save_rxhash(sk, skb);
 
 	if (tcp_rcv_state_process(sk, skb))
 		goto reset;
 	if (opt_skb)
 		goto ipv6_pktoptions;
 	return 0;
 
 reset:
 	tcp_v6_send_reset(sk, skb);
 discard:
 	if (opt_skb)
 		__kfree_skb(opt_skb);
 	kfree_skb(skb);
 	return 0;
 csum_err:
 	TCP_INC_STATS(sock_net(sk), TCP_MIB_CSUMERRORS);
 	TCP_INC_STATS(sock_net(sk), TCP_MIB_INERRS);
 	goto discard;
 
 
 ipv6_pktoptions:
 	 
 	tp = tcp_sk(sk);
 	if (TCP_SKB_CB(opt_skb)->end_seq == tp->rcv_nxt &&
 	    !((1 << sk->sk_state) & (TCPF_CLOSE | TCPF_LISTEN))) {
 		if (np->rxopt.bits.rxinfo || np->rxopt.bits.rxoinfo)
 			np->mcast_oif = tcp_v6_iif(opt_skb);
 		if (np->rxopt.bits.rxhlim || np->rxopt.bits.rxohlim)
 			np->mcast_hops = ipv6_hdr(opt_skb)->hop_limit;
 		if (np->rxopt.bits.rxflow || np->rxopt.bits.rxtclass)
 			np->rcv_flowinfo = ip6_flowinfo(ipv6_hdr(opt_skb));
 		if (np->repflow)
 			np->flow_label = ip6_flowlabel(ipv6_hdr(opt_skb));
 		if (ipv6_opt_accepted(sk, opt_skb, &TCP_SKB_CB(opt_skb)->header.h6)) {
 			skb_set_owner_r(opt_skb, sk);
 			tcp_v6_restore_cb(opt_skb);
 			opt_skb = xchg(&np->pktoptions, opt_skb);
 		} else {
 			__kfree_skb(opt_skb);
 			opt_skb = xchg(&np->pktoptions, NULL);
 		}
 	}
 
 	kfree_skb(opt_skb);
 	return 0;
 }