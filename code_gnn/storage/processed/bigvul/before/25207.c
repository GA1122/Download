 int __inet6_hash(struct sock *sk, struct inet_timewait_sock *tw)
{
	struct inet_hashinfo *hashinfo = sk->sk_prot->h.hashinfo;
	int twrefcnt = 0;

	WARN_ON(!sk_unhashed(sk));

	if (sk->sk_state == TCP_LISTEN) {
		struct inet_listen_hashbucket *ilb;

		ilb = &hashinfo->listening_hash[inet_sk_listen_hashfn(sk)];
		spin_lock(&ilb->lock);
		__sk_nulls_add_node_rcu(sk, &ilb->head);
		spin_unlock(&ilb->lock);
	} else {
		unsigned int hash;
		struct hlist_nulls_head *list;
		spinlock_t *lock;

		sk->sk_hash = hash = inet6_sk_ehashfn(sk);
		list = &inet_ehash_bucket(hashinfo, hash)->chain;
		lock = inet_ehash_lockp(hashinfo, hash);
		spin_lock(lock);
		__sk_nulls_add_node_rcu(sk, list);
		if (tw) {
			WARN_ON(sk->sk_hash != tw->tw_hash);
			twrefcnt = inet_twsk_unhash(tw);
		}
		spin_unlock(lock);
	}

	sock_prot_inuse_add(sock_net(sk), sk->sk_prot, 1);
	return twrefcnt;
}
