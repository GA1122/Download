 int inet6_csk_xmit(struct sock *sk, struct sk_buff *skb, struct flowi *fl_unused)
 {
 	struct ipv6_pinfo *np = inet6_sk(sk);
 	struct flowi6 fl6;
 	struct dst_entry *dst;
 	int res;
 
 	dst = inet6_csk_route_socket(sk, &fl6);
 	if (IS_ERR(dst)) {
 		sk->sk_err_soft = -PTR_ERR(dst);
 		sk->sk_route_caps = 0;
 		kfree_skb(skb);
 		return PTR_ERR(dst);
 	}
 
 	rcu_read_lock();
 	skb_dst_set_noref(skb, dst);
 
  	 
  	fl6.daddr = sk->sk_v6_daddr;
  
	res = ip6_xmit(sk, skb, &fl6, np->opt, np->tclass);
// 	res = ip6_xmit(sk, skb, &fl6, rcu_dereference(np->opt),
// 		       np->tclass);
  	rcu_read_unlock();
  	return res;
  }