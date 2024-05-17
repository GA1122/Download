int inet_listen(struct socket *sock, int backlog)
{
	struct sock *sk = sock->sk;
	unsigned char old_state;
	int err;

	lock_sock(sk);

	err = -EINVAL;
	if (sock->state != SS_UNCONNECTED || sock->type != SOCK_STREAM)
		goto out;

	old_state = sk->sk_state;
	if (!((1 << old_state) & (TCPF_CLOSE | TCPF_LISTEN)))
		goto out;

	 
	if (old_state != TCP_LISTEN) {
		 
		if ((sysctl_tcp_fastopen & TFO_SERVER_ENABLE) != 0 &&
		    !inet_csk(sk)->icsk_accept_queue.fastopenq.max_qlen) {
			if ((sysctl_tcp_fastopen & TFO_SERVER_WO_SOCKOPT1) != 0)
				fastopen_queue_tune(sk, backlog);
			else if ((sysctl_tcp_fastopen &
				  TFO_SERVER_WO_SOCKOPT2) != 0)
				fastopen_queue_tune(sk,
				    ((uint)sysctl_tcp_fastopen) >> 16);

			tcp_fastopen_init_key_once(true);
		}
		err = inet_csk_listen_start(sk, backlog);
		if (err)
			goto out;
	}
	sk->sk_max_ack_backlog = backlog;
	err = 0;

out:
	release_sock(sk);
	return err;
}