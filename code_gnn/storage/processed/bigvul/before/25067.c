int __inet_inherit_port(struct sock *sk, struct sock *child)
{
	struct inet_hashinfo *table = sk->sk_prot->h.hashinfo;
	unsigned short port = inet_sk(child)->inet_num;
	const int bhash = inet_bhashfn(sock_net(sk), port,
			table->bhash_size);
	struct inet_bind_hashbucket *head = &table->bhash[bhash];
	struct inet_bind_bucket *tb;

	spin_lock(&head->lock);
	tb = inet_csk(sk)->icsk_bind_hash;
	if (tb->port != port) {
		 
		struct hlist_node *node;
		inet_bind_bucket_for_each(tb, node, &head->chain) {
			if (net_eq(ib_net(tb), sock_net(sk)) &&
			    tb->port == port)
				break;
		}
		if (!node) {
			tb = inet_bind_bucket_create(table->bind_bucket_cachep,
						     sock_net(sk), head, port);
			if (!tb) {
				spin_unlock(&head->lock);
				return -ENOMEM;
			}
		}
	}
	inet_bind_hash(child, tb, port);
	spin_unlock(&head->lock);

	return 0;
}
