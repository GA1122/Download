struct sock *inet6_lookup_listener(struct net *net,
		struct inet_hashinfo *hashinfo, const struct in6_addr *daddr,
		const unsigned short hnum, const int dif)
{
	struct sock *sk;
	const struct hlist_nulls_node *node;
	struct sock *result;
	int score, hiscore;
	unsigned int hash = inet_lhashfn(net, hnum);
	struct inet_listen_hashbucket *ilb = &hashinfo->listening_hash[hash];

	rcu_read_lock();
begin:
	result = NULL;
	hiscore = -1;
	sk_nulls_for_each(sk, node, &ilb->head) {
		score = compute_score(sk, net, hnum, daddr, dif);
		if (score > hiscore) {
			hiscore = score;
			result = sk;
		}
	}
	 
	if (get_nulls_value(node) != hash + LISTENING_NULLS_BASE)
		goto begin;
	if (result) {
		if (unlikely(!atomic_inc_not_zero(&result->sk_refcnt)))
			result = NULL;
		else if (unlikely(compute_score(result, net, hnum, daddr,
				  dif) < hiscore)) {
			sock_put(result);
			goto begin;
		}
	}
	rcu_read_unlock();
	return result;
}
