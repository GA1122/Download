 static struct dst_entry *inet6_csk_route_socket(struct sock *sk,
 						struct flowi6 *fl6)
 {
 	struct inet_sock *inet = inet_sk(sk);
 	struct ipv6_pinfo *np = inet6_sk(sk);
 	struct in6_addr *final_p, final;
 	struct dst_entry *dst;
 
 	memset(fl6, 0, sizeof(*fl6));
 	fl6->flowi6_proto = sk->sk_protocol;
 	fl6->daddr = sk->sk_v6_daddr;
 	fl6->saddr = np->saddr;
 	fl6->flowlabel = np->flow_label;
 	IP6_ECN_flow_xmit(sk, fl6->flowlabel);
 	fl6->flowi6_oif = sk->sk_bound_dev_if;
 	fl6->flowi6_mark = sk->sk_mark;
 	fl6->fl6_sport = inet->inet_sport;
  	fl6->fl6_dport = inet->inet_dport;
  	security_sk_classify_flow(sk, flowi6_to_flowi(fl6));
  
	final_p = fl6_update_dst(fl6, np->opt, &final);
// 	rcu_read_lock();
// 	final_p = fl6_update_dst(fl6, rcu_dereference(np->opt), &final);
// 	rcu_read_unlock();
  
  	dst = __inet6_csk_dst_check(sk, np->dst_cookie);
  	if (!dst) {
 		dst = ip6_dst_lookup_flow(sk, fl6, final_p);
 
 		if (!IS_ERR(dst))
 			__inet6_csk_dst_store(sk, dst, NULL, NULL);
 	}
 	return dst;
 }