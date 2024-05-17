static int connect(struct socket *sock, struct sockaddr *dest, int destlen,
		   int flags)
{
	struct sock *sk = sock->sk;
	struct sockaddr_tipc *dst = (struct sockaddr_tipc *)dest;
	struct msghdr m = {NULL,};
	unsigned int timeout;
	int res;

	lock_sock(sk);

	 
	if (sock->state == SS_READY) {
		res = -EOPNOTSUPP;
		goto exit;
	}

	 
	if (dst->addrtype == TIPC_ADDR_MCAST) {
		res = -EINVAL;
		goto exit;
	}

	timeout = (flags & O_NONBLOCK) ? 0 : tipc_sk(sk)->conn_timeout;

	switch (sock->state) {
	case SS_UNCONNECTED:
		 
		m.msg_name = dest;
		m.msg_namelen = destlen;

		 
		if (!timeout)
			m.msg_flags = MSG_DONTWAIT;

		res = send_msg(NULL, sock, &m, 0);
		if ((res < 0) && (res != -EWOULDBLOCK))
			goto exit;

		 
		res = -EINPROGRESS;
		break;
	case SS_CONNECTING:
		res = -EALREADY;
		break;
	case SS_CONNECTED:
		res = -EISCONN;
		break;
	default:
		res = -EINVAL;
		goto exit;
	}

	if (sock->state == SS_CONNECTING) {
		if (!timeout)
			goto exit;

		 
		release_sock(sk);
		res = wait_event_interruptible_timeout(*sk_sleep(sk),
				sock->state != SS_CONNECTING,
				timeout ? (long)msecs_to_jiffies(timeout)
					: MAX_SCHEDULE_TIMEOUT);
		lock_sock(sk);
		if (res <= 0) {
			if (res == 0)
				res = -ETIMEDOUT;
			else
				;  
			goto exit;
		}
	}

	if (unlikely(sock->state == SS_DISCONNECTING))
		res = sock_error(sk);
	else
		res = 0;

exit:
	release_sock(sk);
	return res;
}
