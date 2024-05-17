unsigned int sctp_poll(struct file *file, struct socket *sock, poll_table *wait)
{
	struct sock *sk = sock->sk;
	struct sctp_sock *sp = sctp_sk(sk);
	unsigned int mask;

	poll_wait(file, sk->sk_sleep, wait);

	 
	if (sctp_style(sk, TCP) && sctp_sstate(sk, LISTENING))
		return (!list_empty(&sp->ep->asocs)) ?
			(POLLIN | POLLRDNORM) : 0;

	mask = 0;

	 
	if (sk->sk_err || !skb_queue_empty(&sk->sk_error_queue))
		mask |= POLLERR;
	if (sk->sk_shutdown & RCV_SHUTDOWN)
		mask |= POLLRDHUP;
	if (sk->sk_shutdown == SHUTDOWN_MASK)
		mask |= POLLHUP;

	 
	if (!skb_queue_empty(&sk->sk_receive_queue) ||
	    (sk->sk_shutdown & RCV_SHUTDOWN))
		mask |= POLLIN | POLLRDNORM;

	 
	if (!sctp_style(sk, UDP) && sctp_sstate(sk, CLOSED))
		return mask;

	 
	if (sctp_writeable(sk)) {
		mask |= POLLOUT | POLLWRNORM;
	} else {
		set_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);
		 
		if (sctp_writeable(sk))
			mask |= POLLOUT | POLLWRNORM;
	}
	return mask;
}
