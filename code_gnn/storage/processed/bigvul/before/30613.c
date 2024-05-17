static void iucv_sock_cleanup_listen(struct sock *parent)
{
	struct sock *sk;

	 
	while ((sk = iucv_accept_dequeue(parent, NULL))) {
		iucv_sock_close(sk);
		iucv_sock_kill(sk);
	}

	parent->sk_state = IUCV_CLOSED;
}
