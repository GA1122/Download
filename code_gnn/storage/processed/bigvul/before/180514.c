 static struct sock *tcp_v6_syn_recv_sock(const struct sock *sk, struct sk_buff *skb,
 					 struct request_sock *req,
 					 struct dst_entry *dst,
 					 struct request_sock *req_unhash,
 					 bool *own_req)
 {
  	struct inet_request_sock *ireq;
  	struct ipv6_pinfo *newnp;
  	const struct ipv6_pinfo *np = inet6_sk(sk);
// 	struct ipv6_txoptions *opt;
  	struct tcp6_sock *newtcp6sk;
  	struct inet_sock *newinet;
  	struct tcp_sock *newtp;
 	struct sock *newsk;
 #ifdef CONFIG_TCP_MD5SIG
 	struct tcp_md5sig_key *key;
 #endif
 	struct flowi6 fl6;
 
 	if (skb->protocol == htons(ETH_P_IP)) {
 		 
 
 		newsk = tcp_v4_syn_recv_sock(sk, skb, req, dst,
 					     req_unhash, own_req);
 
 		if (!newsk)
 			return NULL;
 
 		newtcp6sk = (struct tcp6_sock *)newsk;
 		inet_sk(newsk)->pinet6 = &newtcp6sk->inet6;
 
 		newinet = inet_sk(newsk);
 		newnp = inet6_sk(newsk);
 		newtp = tcp_sk(newsk);
 
 		memcpy(newnp, np, sizeof(struct ipv6_pinfo));
 
 		newnp->saddr = newsk->sk_v6_rcv_saddr;
 
 		inet_csk(newsk)->icsk_af_ops = &ipv6_mapped;
 		newsk->sk_backlog_rcv = tcp_v4_do_rcv;
 #ifdef CONFIG_TCP_MD5SIG
 		newtp->af_specific = &tcp_sock_ipv6_mapped_specific;
 #endif
 
 		newnp->ipv6_ac_list = NULL;
 		newnp->ipv6_fl_list = NULL;
 		newnp->pktoptions  = NULL;
 		newnp->opt	   = NULL;
 		newnp->mcast_oif   = tcp_v6_iif(skb);
 		newnp->mcast_hops  = ipv6_hdr(skb)->hop_limit;
 		newnp->rcv_flowinfo = ip6_flowinfo(ipv6_hdr(skb));
 		if (np->repflow)
 			newnp->flow_label = ip6_flowlabel(ipv6_hdr(skb));
 
 		 
 
 		 
 		tcp_sync_mss(newsk, inet_csk(newsk)->icsk_pmtu_cookie);
 
 		return newsk;
 	}
 
 	ireq = inet_rsk(req);
 
 	if (sk_acceptq_is_full(sk))
 		goto out_overflow;
 
 	if (!dst) {
 		dst = inet6_csk_route_req(sk, &fl6, req, IPPROTO_TCP);
 		if (!dst)
 			goto out;
 	}
 
 	newsk = tcp_create_openreq_child(sk, req, skb);
 	if (!newsk)
 		goto out_nonewsk;
 
 	 
 
 	newsk->sk_gso_type = SKB_GSO_TCPV6;
 	__ip6_dst_store(newsk, dst, NULL, NULL);
 	inet6_sk_rx_dst_set(newsk, skb);
 
 	newtcp6sk = (struct tcp6_sock *)newsk;
 	inet_sk(newsk)->pinet6 = &newtcp6sk->inet6;
 
 	newtp = tcp_sk(newsk);
 	newinet = inet_sk(newsk);
 	newnp = inet6_sk(newsk);
 
 	memcpy(newnp, np, sizeof(struct ipv6_pinfo));
 
 	newsk->sk_v6_daddr = ireq->ir_v6_rmt_addr;
 	newnp->saddr = ireq->ir_v6_loc_addr;
 	newsk->sk_v6_rcv_saddr = ireq->ir_v6_loc_addr;
 	newsk->sk_bound_dev_if = ireq->ir_iif;
 
 	 
 	newinet->inet_opt = NULL;
 	newnp->ipv6_ac_list = NULL;
 	newnp->ipv6_fl_list = NULL;
 
 	 
 	newnp->rxopt.all = np->rxopt.all;
 
 	newnp->pktoptions = NULL;
 	newnp->opt	  = NULL;
 	newnp->mcast_oif  = tcp_v6_iif(skb);
 	newnp->mcast_hops = ipv6_hdr(skb)->hop_limit;
 	newnp->rcv_flowinfo = ip6_flowinfo(ipv6_hdr(skb));
 	if (np->repflow)
 		newnp->flow_label = ip6_flowlabel(ipv6_hdr(skb));
 
 	 
	if (np->opt)
		newnp->opt = ipv6_dup_options(newsk, np->opt);
// 	opt = rcu_dereference(np->opt);
// 	if (opt) {
// 		opt = ipv6_dup_options(newsk, opt);
// 		RCU_INIT_POINTER(newnp->opt, opt);
// 	}
  	inet_csk(newsk)->icsk_ext_hdr_len = 0;
	if (newnp->opt)
		inet_csk(newsk)->icsk_ext_hdr_len = (newnp->opt->opt_nflen +
						     newnp->opt->opt_flen);
// 	if (opt)
// 		inet_csk(newsk)->icsk_ext_hdr_len = opt->opt_nflen +
// 						    opt->opt_flen;
  
  	tcp_ca_openreq_child(newsk, dst);
  
 	tcp_sync_mss(newsk, dst_mtu(dst));
 	newtp->advmss = dst_metric_advmss(dst);
 	if (tcp_sk(sk)->rx_opt.user_mss &&
 	    tcp_sk(sk)->rx_opt.user_mss < newtp->advmss)
 		newtp->advmss = tcp_sk(sk)->rx_opt.user_mss;
 
 	tcp_initialize_rcv_mss(newsk);
 
 	newinet->inet_daddr = newinet->inet_saddr = LOOPBACK4_IPV6;
 	newinet->inet_rcv_saddr = LOOPBACK4_IPV6;
 
 #ifdef CONFIG_TCP_MD5SIG
 	 
 	key = tcp_v6_md5_do_lookup(sk, &newsk->sk_v6_daddr);
 	if (key) {
 		 
 		tcp_md5_do_add(newsk, (union tcp_md5_addr *)&newsk->sk_v6_daddr,
 			       AF_INET6, key->key, key->keylen,
 			       sk_gfp_atomic(sk, GFP_ATOMIC));
 	}
 #endif
 
 	if (__inet_inherit_port(sk, newsk) < 0) {
 		inet_csk_prepare_forced_close(newsk);
 		tcp_done(newsk);
 		goto out;
 	}
 	*own_req = inet_ehash_nolisten(newsk, req_to_sk(req_unhash));
 	if (*own_req) {
 		tcp_move_syn(newtp, req);
 
 		 
 		if (ireq->pktopts) {
 			newnp->pktoptions = skb_clone(ireq->pktopts,
 						      sk_gfp_atomic(sk, GFP_ATOMIC));
 			consume_skb(ireq->pktopts);
 			ireq->pktopts = NULL;
 			if (newnp->pktoptions)
 				skb_set_owner_r(newnp->pktoptions, newsk);
 		}
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