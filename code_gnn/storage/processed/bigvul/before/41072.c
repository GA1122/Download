void sk_clear_memalloc(struct sock *sk)
{
	sock_reset_flag(sk, SOCK_MEMALLOC);
	sk->sk_allocation &= ~__GFP_MEMALLOC;
	static_key_slow_dec(&memalloc_socks);

	 
	if (WARN_ON(sk->sk_forward_alloc))
		sk_mem_reclaim(sk);
}
