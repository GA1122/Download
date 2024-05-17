void sock_enable_timestamp(struct sock *sk, int flag)
{
	if (!sock_flag(sk, flag)) {
		unsigned long previous_flags = sk->sk_flags;

		sock_set_flag(sk, flag);
		 
		if (sock_needs_netstamp(sk) &&
		    !(previous_flags & SK_FLAGS_TIMESTAMP))
			net_enable_timestamp();
	}
}
