static int __inet6_check_established(struct inet_timewait_death_row *death_row,
				     struct sock *sk, const __u16 lport,
				     struct inet_timewait_sock **twp)
{
	struct inet_hashinfo *hinfo = death_row->hashinfo;
	struct inet_sock *inet = inet_sk(sk);
	const struct ipv6_pinfo *np = inet6_sk(sk);
	const struct in6_addr *daddr = &np->rcv_saddr;
	const struct in6_addr *saddr = &np->daddr;
	const int dif = sk->sk_bound_dev_if;
	const __portpair ports = INET_COMBINED_PORTS(inet->inet_dport, lport);
	struct net *net = sock_net(sk);
	const unsigned int hash = inet6_ehashfn(net, daddr, lport, saddr,
						inet->inet_dport);
	struct inet_ehash_bucket *head = inet_ehash_bucket(hinfo, hash);
	spinlock_t *lock = inet_ehash_lockp(hinfo, hash);
	struct sock *sk2;
	const struct hlist_nulls_node *node;
	struct inet_timewait_sock *tw;
	int twrefcnt = 0;

	spin_lock(lock);

	 
	sk_nulls_for_each(sk2, node, &head->twchain) {
		tw = inet_twsk(sk2);

		if (INET6_TW_MATCH(sk2, net, hash, saddr, daddr, ports, dif)) {
			if (twsk_unique(sk, sk2, twp))
				goto unique;
			else
				goto not_unique;
		}
	}
	tw = NULL;

	 
	sk_nulls_for_each(sk2, node, &head->chain) {
		if (INET6_MATCH(sk2, net, hash, saddr, daddr, ports, dif))
			goto not_unique;
	}

unique:
	 
	inet->inet_num = lport;
	inet->inet_sport = htons(lport);
	sk->sk_hash = hash;
	WARN_ON(!sk_unhashed(sk));
	__sk_nulls_add_node_rcu(sk, &head->chain);
	if (tw) {
		twrefcnt = inet_twsk_unhash(tw);
		NET_INC_STATS_BH(net, LINUX_MIB_TIMEWAITRECYCLED);
	}
	spin_unlock(lock);
	if (twrefcnt)
		inet_twsk_put(tw);
	sock_prot_inuse_add(sock_net(sk), sk->sk_prot, 1);

	if (twp) {
		*twp = tw;
	} else if (tw) {
		 
		inet_twsk_deschedule(tw, death_row);

		inet_twsk_put(tw);
	}
	return 0;

not_unique:
	spin_unlock(lock);
	return -EADDRNOTAVAIL;
}