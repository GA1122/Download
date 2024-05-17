static void tcp_v6_destroy_sock(struct sock *sk)
{
#ifdef CONFIG_TCP_MD5SIG
	 
	if (tcp_sk(sk)->md5sig_info)
		tcp_v6_clear_md5_list(sk);
#endif
	tcp_v4_destroy_sock(sk);
	inet6_destroy_sock(sk);
}