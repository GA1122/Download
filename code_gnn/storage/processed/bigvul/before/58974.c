static int l2cap_sock_sendmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	int err = 0;

	BT_DBG("sock %p, sk %p", sock, sk);

	err = sock_error(sk);
	if (err)
		return err;

	if (msg->msg_flags & MSG_OOB)
		return -EOPNOTSUPP;

	 
	if (sk->sk_type != SOCK_RAW && len > l2cap_pi(sk)->omtu)
		return -EINVAL;

	lock_sock(sk);

	if (sk->sk_state == BT_CONNECTED)
		err = l2cap_do_send(sk, msg, len);
	else
		err = -ENOTCONN;

	release_sock(sk);
	return err;
}