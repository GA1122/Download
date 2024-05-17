 static struct sock *dccp_v6_request_recv_sock(const struct sock *sk,
 					      struct sk_buff *skb,
 					      struct request_sock *req,
 					      struct dst_entry *dst,
 					      struct request_sock *req_unhash,
 					      bool *own_req)
 {
  	struct inet_request_sock *ireq = inet_rsk(req);
  	struct ipv6_pinfo *newnp;
  	const struct ipv6_pinfo *np = inet6_sk(sk);
// 	struct ipv6_txoptions *opt;
  	struct inet_sock *newinet;
  	struct dccp6_sock *newdp6;
  	struct sock *newsk;
 
 	if (skb->protocol == htons(ETH_P_IP)) {
 		 
 		newsk = dccp_v4_request_recv_sock(sk, skb, req, dst,
 						  req_unhash, own_req);
 		if (newsk == NULL)
 			return NULL;
 
 		newdp6 = (struct dccp6_sock *)newsk;
 		newinet = inet_sk(newsk);
 		newinet->pinet6 = &newdp6->inet6;
 		newnp = inet6_sk(newsk);
 
 		memcpy(newnp, np, sizeof(struct ipv6_pinfo));
 
 		newnp->saddr = newsk->sk_v6_rcv_saddr;
 
 		inet_csk(newsk)->icsk_af_ops = &dccp_ipv6_mapped;
 		newsk->sk_backlog_rcv = dccp_v4_do_rcv;
 		newnp->pktoptions  = NULL;
 		newnp->opt	   = NULL;
 		newnp->mcast_oif   = inet6_iif(skb);
 		newnp->mcast_hops  = ipv6_hdr(skb)->hop_limit;
 
 		 
 
 		 
 		dccp_sync_mss(newsk, inet_csk(newsk)->icsk_pmtu_cookie);
 
 		return newsk;
 	}
 
 
 	if (sk_acceptq_is_full(sk))
 		goto out_overflow;
 
 	if (!dst) {
 		struct flowi6 fl6;
 
 		dst = inet6_csk_route_req(sk, &fl6, req, IPPROTO_DCCP);
 		if (!dst)
 			goto out;
 	}
 
 	newsk = dccp_create_openreq_child(sk, req, skb);
 	if (newsk == NULL)
 		goto out_nonewsk;
 
 	 
 
 	__ip6_dst_store(newsk, dst, NULL, NULL);
 	newsk->sk_route_caps = dst->dev->features & ~(NETIF_F_IP_CSUM |
 						      NETIF_F_TSO);
 	newdp6 = (struct dccp6_sock *)newsk;
 	newinet = inet_sk(newsk);
 	newinet->pinet6 = &newdp6->inet6;
 	newnp = inet6_sk(newsk);
 
 	memcpy(newnp, np, sizeof(struct ipv6_pinfo));
 
 	newsk->sk_v6_daddr	= ireq->ir_v6_rmt_addr;
 	newnp->saddr		= ireq->ir_v6_loc_addr;
 	newsk->sk_v6_rcv_saddr	= ireq->ir_v6_loc_addr;
 	newsk->sk_bound_dev_if	= ireq->ir_iif;
 
 	 
 	newinet->inet_opt = NULL;
 
 	 
 	newnp->rxopt.all = np->rxopt.all;
 
 	newnp->pktoptions = NULL;
 	newnp->opt	  = NULL;
 	newnp->mcast_oif  = inet6_iif(skb);
 	newnp->mcast_hops = ipv6_hdr(skb)->hop_limit;
 
 	 
	if (np->opt != NULL)
		newnp->opt = ipv6_dup_options(newsk, np->opt);
// 	opt = rcu_dereference(np->opt);
// 	if (opt) {
// 		opt = ipv6_dup_options(newsk, opt);
// 		RCU_INIT_POINTER(newnp->opt, opt);
// 	}
  	inet_csk(newsk)->icsk_ext_hdr_len = 0;
	if (newnp->opt != NULL)
		inet_csk(newsk)->icsk_ext_hdr_len = (newnp->opt->opt_nflen +
						     newnp->opt->opt_flen);
// 	if (opt)
// 		inet_csk(newsk)->icsk_ext_hdr_len = opt->opt_nflen +
// 						    opt->opt_flen;
  
  	dccp_sync_mss(newsk, dst_mtu(dst));
  
 	newinet->inet_daddr = newinet->inet_saddr = LOOPBACK4_IPV6;
 	newinet->inet_rcv_saddr = LOOPBACK4_IPV6;
 
 	if (__inet_inherit_port(sk, newsk) < 0) {
 		inet_csk_prepare_forced_close(newsk);
 		dccp_done(newsk);
 		goto out;
 	}
 	*own_req = inet_ehash_nolisten(newsk, req_to_sk(req_unhash));
 	 
 	if (*own_req && ireq->pktopts) {
 		newnp->pktoptions = skb_clone(ireq->pktopts, GFP_ATOMIC);
 		consume_skb(ireq->pktopts);
 		ireq->pktopts = NULL;
 		if (newnp->pktoptions)
 			skb_set_owner_r(newnp->pktoptions, newsk);
 	}
 
 	return newsk;
 
 out_overflow:
 	NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
 out_nonewsk:
 	dst_release(dst);
 out:
 	NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENDROPS);
 	return NULL;
 }