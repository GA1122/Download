static void netlink_sock_destruct(struct sock *sk)
{
	struct netlink_sock *nlk = nlk_sk(sk);

	if (nlk->cb) {
		if (nlk->cb->done)
			nlk->cb->done(nlk->cb);
		netlink_destroy_callback(nlk->cb);
	}

	skb_queue_purge(&sk->sk_receive_queue);

	if (!sock_flag(sk, SOCK_DEAD)) {
		printk(KERN_ERR "Freeing alive netlink socket %p\n", sk);
		return;
	}

	WARN_ON(atomic_read(&sk->sk_rmem_alloc));
	WARN_ON(atomic_read(&sk->sk_wmem_alloc));
	WARN_ON(nlk_sk(sk)->groups);
}