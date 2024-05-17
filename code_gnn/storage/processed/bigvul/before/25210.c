int inet6_hash_connect(struct inet_timewait_death_row *death_row,
		       struct sock *sk)
{
	return __inet_hash_connect(death_row, sk, inet6_sk_port_offset(sk),
			__inet6_check_established, __inet6_hash);
}
