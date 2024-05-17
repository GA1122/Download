static int l2tp_ip_open(struct sock *sk)
{
	 
	inet_sk(sk)->inet_num = IPPROTO_L2TP;

	write_lock_bh(&l2tp_ip_lock);
	sk_add_node(sk, &l2tp_ip_table);
	write_unlock_bh(&l2tp_ip_lock);

	return 0;
}
