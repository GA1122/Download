struct sock * __inet_lookup_established(struct net *net,
				  struct inet_hashinfo *hashinfo,
				  const __be32 saddr, const __be16 sport,
				  const __be32 daddr, const u16 hnum,
				  const int dif)
{
	INET_ADDR_COOKIE(acookie, saddr, daddr)
	const __portpair ports = INET_COMBINED_PORTS(sport, hnum);
	struct sock *sk;
	const struct hlist_nulls_node *node;
	 
	unsigned int hash = inet_ehashfn(net, daddr, hnum, saddr, sport);
	unsigned int slot = hash & hashinfo->ehash_mask;
	struct inet_ehash_bucket *head = &hashinfo->ehash[slot];

	rcu_read_lock();
begin:
	sk_nulls_for_each_rcu(sk, node, &head->chain) {
		if (INET_MATCH(sk, net, hash, acookie,
					saddr, daddr, ports, dif)) {
			if (unlikely(!atomic_inc_not_zero(&sk->sk_refcnt)))
				goto begintw;
			if (unlikely(!INET_MATCH(sk, net, hash, acookie,
				saddr, daddr, ports, dif))) {
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
		if (INET_TW_MATCH(sk, net, hash, acookie,
					saddr, daddr, ports, dif)) {
			if (unlikely(!atomic_inc_not_zero(&sk->sk_refcnt))) {
				sk = NULL;
				goto out;
			}
			if (unlikely(!INET_TW_MATCH(sk, net, hash, acookie,
				 saddr, daddr, ports, dif))) {
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
