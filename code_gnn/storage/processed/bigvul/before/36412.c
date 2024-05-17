static int pppol2tp_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct l2tp_session *session;
	int error;

	if (!sk)
		return 0;

	error = -EBADF;
	lock_sock(sk);
	if (sock_flag(sk, SOCK_DEAD) != 0)
		goto error;

	pppox_unbind_sock(sk);

	 
	sk->sk_state = PPPOX_DEAD;
	sock_orphan(sk);
	sock->sk = NULL;

	session = pppol2tp_sock_to_session(sk);

	 
	if (session != NULL) {
		__l2tp_session_unhash(session);
		l2tp_session_queue_purge(session);
		sock_put(sk);
	}
	skb_queue_purge(&sk->sk_receive_queue);
	skb_queue_purge(&sk->sk_write_queue);

	release_sock(sk);

	 
	sock_put(sk);

	return 0;

error:
	release_sock(sk);
	return error;
}
