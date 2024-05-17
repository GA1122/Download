void inet_unhash(struct sock *sk)
{
	struct inet_hashinfo *hashinfo = sk->sk_prot->h.hashinfo;
	spinlock_t *lock;
	int done;

	if (sk_unhashed(sk))
		return;

	if (sk->sk_state == TCP_LISTEN)
		lock = &hashinfo->listening_hash[inet_sk_listen_hashfn(sk)].lock;
	else
		lock = inet_ehash_lockp(hashinfo, sk->sk_hash);

	spin_lock_bh(lock);
	done =__sk_nulls_del_node_init_rcu(sk);
	if (done)
		sock_prot_inuse_add(sock_net(sk), sk->sk_prot, -1);
	spin_unlock_bh(lock);
}
