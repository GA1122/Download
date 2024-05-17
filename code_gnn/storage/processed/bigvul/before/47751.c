static int netlink_insert(struct sock *sk, u32 portid)
{
	struct netlink_table *table = &nl_table[sk->sk_protocol];
	int err;

	lock_sock(sk);

	err = nlk_sk(sk)->portid == portid ? 0 : -EBUSY;
	if (nlk_sk(sk)->bound)
		goto err;

	err = -ENOMEM;
	if (BITS_PER_LONG > 32 &&
	    unlikely(atomic_read(&table->hash.nelems) >= UINT_MAX))
		goto err;

	nlk_sk(sk)->portid = portid;
	sock_hold(sk);

	err = __netlink_insert(table, sk);
	if (err) {
		 
		if (unlikely(err == -EBUSY))
			err = -EOVERFLOW;
		if (err == -EEXIST)
			err = -EADDRINUSE;
		sock_put(sk);
		goto err;
	}

	 
	smp_wmb();
	nlk_sk(sk)->bound = portid;

err:
	release_sock(sk);
	return err;
}
