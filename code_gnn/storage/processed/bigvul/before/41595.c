static int irda_release(struct socket *sock)
{
	struct sock *sk = sock->sk;

	if (sk == NULL)
		return 0;

	lock_sock(sk);
	sk->sk_state       = TCP_CLOSE;
	sk->sk_shutdown   |= SEND_SHUTDOWN;
	sk->sk_state_change(sk);

	 
	irda_destroy_socket(irda_sk(sk));

	sock_orphan(sk);
	sock->sk   = NULL;
	release_sock(sk);

	 
	skb_queue_purge(&sk->sk_receive_queue);

	 
	sock_put(sk);

	 

	return 0;
}
