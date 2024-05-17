static unsigned int irda_poll(struct file * file, struct socket *sock,
			      poll_table *wait)
{
	struct sock *sk = sock->sk;
	struct irda_sock *self = irda_sk(sk);
	unsigned int mask;

	poll_wait(file, sk_sleep(sk), wait);
	mask = 0;

	 
	if (sk->sk_err)
		mask |= POLLERR;
	if (sk->sk_shutdown & RCV_SHUTDOWN) {
		pr_debug("%s(), POLLHUP\n", __func__);
		mask |= POLLHUP;
	}

	 
	if (!skb_queue_empty(&sk->sk_receive_queue)) {
		pr_debug("Socket is readable\n");
		mask |= POLLIN | POLLRDNORM;
	}

	 
	switch (sk->sk_type) {
	case SOCK_STREAM:
		if (sk->sk_state == TCP_CLOSE) {
			pr_debug("%s(), POLLHUP\n", __func__);
			mask |= POLLHUP;
		}

		if (sk->sk_state == TCP_ESTABLISHED) {
			if ((self->tx_flow == FLOW_START) &&
			    sock_writeable(sk))
			{
				mask |= POLLOUT | POLLWRNORM | POLLWRBAND;
			}
		}
		break;
	case SOCK_SEQPACKET:
		if ((self->tx_flow == FLOW_START) &&
		    sock_writeable(sk))
		{
			mask |= POLLOUT | POLLWRNORM | POLLWRBAND;
		}
		break;
	case SOCK_DGRAM:
		if (sock_writeable(sk))
			mask |= POLLOUT | POLLWRNORM | POLLWRBAND;
		break;
	default:
		break;
	}

	return mask;
}