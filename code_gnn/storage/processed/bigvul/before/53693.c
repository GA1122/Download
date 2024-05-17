int inet6_csk_bind_conflict(const struct sock *sk,
			    const struct inet_bind_bucket *tb, bool relax)
{
	const struct sock *sk2;
	int reuse = sk->sk_reuse;
	int reuseport = sk->sk_reuseport;
	kuid_t uid = sock_i_uid((struct sock *)sk);

	 
	 
	sk_for_each_bound(sk2, &tb->owners) {
		if (sk != sk2 &&
		    (!sk->sk_bound_dev_if ||
		     !sk2->sk_bound_dev_if ||
		     sk->sk_bound_dev_if == sk2->sk_bound_dev_if)) {
			if ((!reuse || !sk2->sk_reuse ||
			     sk2->sk_state == TCP_LISTEN) &&
			    (!reuseport || !sk2->sk_reuseport ||
			     (sk2->sk_state != TCP_TIME_WAIT &&
			      !uid_eq(uid,
				      sock_i_uid((struct sock *)sk2))))) {
				if (ipv6_rcv_saddr_equal(sk, sk2))
					break;
			}
			if (!relax && reuse && sk2->sk_reuse &&
			    sk2->sk_state != TCP_LISTEN &&
			    ipv6_rcv_saddr_equal(sk, sk2))
				break;
		}
	}

	return sk2 != NULL;
}