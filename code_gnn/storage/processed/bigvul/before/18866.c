static inline struct sock *icmp_xmit_lock(struct net *net)
{
	struct sock *sk;

	local_bh_disable();

	sk = icmp_sk(net);

	if (unlikely(!spin_trylock(&sk->sk_lock.slock))) {
		 
		local_bh_enable();
		return NULL;
	}
	return sk;
}
