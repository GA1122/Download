void sk_common_release(struct sock *sk)
{
	if (sk->sk_prot->destroy)
		sk->sk_prot->destroy(sk);

	 

	sk->sk_prot->unhash(sk);

	 

	sock_orphan(sk);

	xfrm_sk_free_policy(sk);

	sk_refcnt_debug_release(sk);
	sock_put(sk);
}
