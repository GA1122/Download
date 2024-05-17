static int vsock_shutdown(struct socket *sock, int mode)
{
	int err;
	struct sock *sk;

	 
	mode++;

	if ((mode & ~SHUTDOWN_MASK) || !mode)
		return -EINVAL;

	 

	sk = sock->sk;
	if (sock->state == SS_UNCONNECTED) {
		err = -ENOTCONN;
		if (sk->sk_type == SOCK_STREAM)
			return err;
	} else {
		sock->state = SS_DISCONNECTING;
		err = 0;
	}

	 
	mode = mode & (RCV_SHUTDOWN | SEND_SHUTDOWN);
	if (mode) {
		lock_sock(sk);
		sk->sk_shutdown |= mode;
		sk->sk_state_change(sk);
		release_sock(sk);

		if (sk->sk_type == SOCK_STREAM) {
			sock_reset_flag(sk, SOCK_DONE);
			vsock_send_shutdown(sk, mode);
		}
	}

	return err;
}
