 static int l2tp_ip_sendmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg, size_t len)
 {
 	struct sk_buff *skb;
  	int rc;
  	struct l2tp_ip_sock *lsa = l2tp_ip_sk(sk);
  	struct inet_sock *inet = inet_sk(sk);
	struct ip_options *opt = inet->opt;
  	struct rtable *rt = NULL;
  	int connected = 0;
  	__be32 daddr;
 
 	if (sock_flag(sk, SOCK_DEAD))
 		return -ENOTCONN;
 
 	 
 	if (msg->msg_name) {
 		struct sockaddr_l2tpip *lip = (struct sockaddr_l2tpip *) msg->msg_name;
 		if (msg->msg_namelen < sizeof(*lip))
 			return -EINVAL;
 
 		if (lip->l2tp_family != AF_INET) {
 			if (lip->l2tp_family != AF_UNSPEC)
 				return -EAFNOSUPPORT;
 		}
 
 		daddr = lip->l2tp_addr.s_addr;
 	} else {
 		if (sk->sk_state != TCP_ESTABLISHED)
 			return -EDESTADDRREQ;
 
 		daddr = inet->inet_daddr;
 		connected = 1;
 	}
 
 	 
 	rc = -ENOMEM;
 	skb = sock_wmalloc(sk, 2 + NET_SKB_PAD + sizeof(struct iphdr) +
 			   4 + len, 0, GFP_KERNEL);
 	if (!skb)
 		goto error;
 
 	 
 	skb_reserve(skb, 2 + NET_SKB_PAD);
 	skb_reset_network_header(skb);
 	skb_reserve(skb, sizeof(struct iphdr));
 	skb_reset_transport_header(skb);
 
 	 
 	*((__be32 *) skb_put(skb, 4)) = 0;
 
 	 
 	rc = memcpy_fromiovec(skb_put(skb, len), msg->msg_iov, len);
 	if (rc < 0) {
 		kfree_skb(skb);
 		goto error;
 	}
 
 	if (connected)
  		rt = (struct rtable *) __sk_dst_check(sk, 0);
  
  	if (rt == NULL) {
// 		struct ip_options_rcu *inet_opt;
// 
// 		inet_opt = rcu_dereference_protected(inet->inet_opt,
// 						     sock_owned_by_user(sk));
// 
  		 
		if (opt && opt->srr)
			daddr = opt->faddr;
// 		if (inet_opt && inet_opt->opt.srr)
// 			daddr = inet_opt->opt.faddr;
  
  		 
 		rt = ip_route_output_ports(sock_net(sk), sk,
 					   daddr, inet->inet_saddr,
 					   inet->inet_dport, inet->inet_sport,
 					   sk->sk_protocol, RT_CONN_FLAGS(sk),
 					   sk->sk_bound_dev_if);
 		if (IS_ERR(rt))
 			goto no_route;
 		sk_setup_caps(sk, &rt->dst);
 	}
 	skb_dst_set(skb, dst_clone(&rt->dst));
 
 	 
 	rc = ip_queue_xmit(skb);
 
 error:
 	 
 	if (rc >= 0) {
 		lsa->tx_packets++;
 		lsa->tx_bytes += len;
 		rc = len;
 	} else {
 		lsa->tx_errors++;
 	}
 
 	return rc;
 
 no_route:
 	IP_INC_STATS(sock_net(sk), IPSTATS_MIB_OUTNOROUTES);
 	kfree_skb(skb);
 	return -EHOSTUNREACH;
 }