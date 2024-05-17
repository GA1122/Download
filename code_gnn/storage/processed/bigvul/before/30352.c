static unsigned int vsock_poll(struct file *file, struct socket *sock,
			       poll_table *wait)
{
	struct sock *sk;
	unsigned int mask;
	struct vsock_sock *vsk;

	sk = sock->sk;
	vsk = vsock_sk(sk);

	poll_wait(file, sk_sleep(sk), wait);
	mask = 0;

	if (sk->sk_err)
		 
		mask |= POLLERR;

	 
	if ((sk->sk_shutdown == SHUTDOWN_MASK) ||
	    ((sk->sk_shutdown & SEND_SHUTDOWN) &&
	     (vsk->peer_shutdown & SEND_SHUTDOWN))) {
		mask |= POLLHUP;
	}

	if (sk->sk_shutdown & RCV_SHUTDOWN ||
	    vsk->peer_shutdown & SEND_SHUTDOWN) {
		mask |= POLLRDHUP;
	}

	if (sock->type == SOCK_DGRAM) {
		 
		if (!skb_queue_empty(&sk->sk_receive_queue) ||
		    (sk->sk_shutdown & RCV_SHUTDOWN)) {
			mask |= POLLIN | POLLRDNORM;
		}

		if (!(sk->sk_shutdown & SEND_SHUTDOWN))
			mask |= POLLOUT | POLLWRNORM | POLLWRBAND;

	} else if (sock->type == SOCK_STREAM) {
		lock_sock(sk);

		 
		if (sk->sk_state == SS_LISTEN
		    && !vsock_is_accept_queue_empty(sk))
			mask |= POLLIN | POLLRDNORM;

		 
		if (transport->stream_is_active(vsk) &&
		    !(sk->sk_shutdown & RCV_SHUTDOWN)) {
			bool data_ready_now = false;
			int ret = transport->notify_poll_in(
					vsk, 1, &data_ready_now);
			if (ret < 0) {
				mask |= POLLERR;
			} else {
				if (data_ready_now)
					mask |= POLLIN | POLLRDNORM;

			}
		}

		 
		if (sk->sk_shutdown & RCV_SHUTDOWN ||
		    vsk->peer_shutdown & SEND_SHUTDOWN) {
			mask |= POLLIN | POLLRDNORM;
		}

		 
		if (sk->sk_state == SS_CONNECTED) {
			if (!(sk->sk_shutdown & SEND_SHUTDOWN)) {
				bool space_avail_now = false;
				int ret = transport->notify_poll_out(
						vsk, 1, &space_avail_now);
				if (ret < 0) {
					mask |= POLLERR;
				} else {
					if (space_avail_now)
						 
						mask |= POLLOUT | POLLWRNORM;

				}
			}
		}

		 
		if (sk->sk_state == SS_UNCONNECTED) {
			if (!(sk->sk_shutdown & SEND_SHUTDOWN))
				mask |= POLLOUT | POLLWRNORM;

		}

		release_sock(sk);
	}

	return mask;
}
