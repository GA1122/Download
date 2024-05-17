static void __inet_hash(struct sock *sk)
{
	struct inet_hashinfo *hashinfo = sk->sk_prot->h.hashinfo;
	struct inet_listen_hashbucket *ilb;

	if (sk->sk_state != TCP_LISTEN) {
		__inet_hash_nolisten(sk, NULL);
		return;
	}

	WARN_ON(!sk_unhashed(sk));
	ilb = &hashinfo->listening_hash[inet_sk_listen_hashfn(sk)];

	spin_lock(&ilb->lock);
	__sk_nulls_add_node_rcu(sk, &ilb->head);
	sock_prot_inuse_add(sock_net(sk), sk->sk_prot, 1);
	spin_unlock(&ilb->lock);
}
