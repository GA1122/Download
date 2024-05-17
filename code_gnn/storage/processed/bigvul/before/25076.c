int inet_hash_connect(struct inet_timewait_death_row *death_row,
		      struct sock *sk)
{
	return __inet_hash_connect(death_row, sk, inet_sk_port_offset(sk),
			__inet_check_established, __inet_hash_nolisten);
}
