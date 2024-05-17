int ping_hash(struct sock *sk)
{
	pr_debug("ping_hash(sk->port=%u)\n", inet_sk(sk)->inet_num);
	BUG();  

	return 0;
}
