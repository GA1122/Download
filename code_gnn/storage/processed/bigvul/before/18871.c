int inet_csk_bind_conflict(const struct sock *sk,
			   const struct inet_bind_bucket *tb)
{
	struct sock *sk2;
	struct hlist_node *node;
	int reuse = sk->sk_reuse;

	 

	sk_for_each_bound(sk2, node, &tb->owners) {
		if (sk != sk2 &&
		    !inet_v6_ipv6only(sk2) &&
		    (!sk->sk_bound_dev_if ||
		     !sk2->sk_bound_dev_if ||
		     sk->sk_bound_dev_if == sk2->sk_bound_dev_if)) {
			if (!reuse || !sk2->sk_reuse ||
			    sk2->sk_state == TCP_LISTEN) {
				const __be32 sk2_rcv_saddr = sk_rcv_saddr(sk2);
				if (!sk2_rcv_saddr || !sk_rcv_saddr(sk) ||
				    sk2_rcv_saddr == sk_rcv_saddr(sk))
					break;
			}
		}
	}
	return node != NULL;
}