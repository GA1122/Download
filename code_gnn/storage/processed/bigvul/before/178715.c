 int inet_sk_rebuild_header(struct sock *sk)
 {
  	struct inet_sock *inet = inet_sk(sk);
  	struct rtable *rt = (struct rtable *)__sk_dst_check(sk, 0);
  	__be32 daddr;
// 	struct ip_options_rcu *inet_opt;
  	int err;
  
  	 
  	if (rt)
  		return 0;
  
  	 
// 	rcu_read_lock();
// 	inet_opt = rcu_dereference(inet->inet_opt);
  	daddr = inet->inet_daddr;
	if (inet->opt && inet->opt->srr)
		daddr = inet->opt->faddr;
// 	if (inet_opt && inet_opt->opt.srr)
// 		daddr = inet_opt->opt.faddr;
// 	rcu_read_unlock();
  	rt = ip_route_output_ports(sock_net(sk), sk, daddr, inet->inet_saddr,
  				   inet->inet_dport, inet->inet_sport,
  				   sk->sk_protocol, RT_CONN_FLAGS(sk),
 				   sk->sk_bound_dev_if);
 	if (!IS_ERR(rt)) {
 		err = 0;
 		sk_setup_caps(sk, &rt->dst);
 	} else {
 		err = PTR_ERR(rt);
 
 		 
 		sk->sk_route_caps = 0;
 		 
 		if (!sysctl_ip_dynaddr ||
 		    sk->sk_state != TCP_SYN_SENT ||
 		    (sk->sk_userlocks & SOCK_BINDADDR_LOCK) ||
 		    (err = inet_sk_reselect_saddr(sk)) != 0)
 			sk->sk_err_soft = -err;
 	}
 
 	return err;
 }