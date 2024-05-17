struct sock *__inet6_lookup_established(struct net *net,
					struct inet_hashinfo *hashinfo,
					   const struct in6_addr *saddr,
					   const __be16 sport,
					   const struct in6_addr *daddr,
					   const u16 hnum,
					   const int dif)
{
	struct sock *sk;
	const struct hlist_nulls_node *node;
	const __portpair ports = INET_COMBINED_PORTS(sport, hnum);
	 
	unsigned int hash = inet6_ehashfn(net, daddr, hnum, saddr, sport);
	unsigned int slot = hash & hashinfo->ehash_mask;
	struct inet_ehash_bucket *head = &hashinfo->ehash[slot];


	rcu_read_lock();
begin:
	sk_nulls_for_each_rcu(sk, node, &head->chain) {
		 
		if (INET6_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
			if (unlikely(!atomic_inc_not_zero(&sk->sk_refcnt)))
				goto begintw;
			if (!INET6_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
				sock_put(sk);
				goto begin;
			}
		goto out;
		}
	}
	if (get_nulls_value(node) != slot)
		goto begin;

begintw:
	 
	sk_nulls_for_each_rcu(sk, node, &head->twchain) {
		if (INET6_TW_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
			if (unlikely(!atomic_inc_not_zero(&sk->sk_refcnt))) {
				sk = NULL;
				goto out;
			}
			if (!INET6_TW_MATCH(sk, net, hash, saddr, daddr, ports, dif)) {
				sock_put(sk);
				goto begintw;
			}
			goto out;
		}
	}
	if (get_nulls_value(node) != slot)
		goto begintw;
	sk = NULL;
out:
	rcu_read_unlock();
	return sk;
}
