int __inet_hash_nolisten(struct sock *sk, struct inet_timewait_sock *tw)
{
	struct inet_hashinfo *hashinfo = sk->sk_prot->h.hashinfo;
	struct hlist_nulls_head *list;
	spinlock_t *lock;
	struct inet_ehash_bucket *head;
	int twrefcnt = 0;

	WARN_ON(!sk_unhashed(sk));

	sk->sk_hash = inet_sk_ehashfn(sk);
	head = inet_ehash_bucket(hashinfo, sk->sk_hash);
	list = &head->chain;
	lock = inet_ehash_lockp(hashinfo, sk->sk_hash);

	spin_lock(lock);
	__sk_nulls_add_node_rcu(sk, list);
	if (tw) {
		WARN_ON(sk->sk_hash != tw->tw_hash);
		twrefcnt = inet_twsk_unhash(tw);
	}
	spin_unlock(lock);
	sock_prot_inuse_add(sock_net(sk), sk->sk_prot, 1);
	return twrefcnt;
}
