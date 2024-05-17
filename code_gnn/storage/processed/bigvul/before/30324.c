static int vsock_accept(struct socket *sock, struct socket *newsock, int flags)
{
	struct sock *listener;
	int err;
	struct sock *connected;
	struct vsock_sock *vconnected;
	long timeout;
	DEFINE_WAIT(wait);

	err = 0;
	listener = sock->sk;

	lock_sock(listener);

	if (sock->type != SOCK_STREAM) {
		err = -EOPNOTSUPP;
		goto out;
	}

	if (listener->sk_state != SS_LISTEN) {
		err = -EINVAL;
		goto out;
	}

	 
	timeout = sock_sndtimeo(listener, flags & O_NONBLOCK);
	prepare_to_wait(sk_sleep(listener), &wait, TASK_INTERRUPTIBLE);

	while ((connected = vsock_dequeue_accept(listener)) == NULL &&
	       listener->sk_err == 0) {
		release_sock(listener);
		timeout = schedule_timeout(timeout);
		lock_sock(listener);

		if (signal_pending(current)) {
			err = sock_intr_errno(timeout);
			goto out_wait;
		} else if (timeout == 0) {
			err = -EAGAIN;
			goto out_wait;
		}

		prepare_to_wait(sk_sleep(listener), &wait, TASK_INTERRUPTIBLE);
	}

	if (listener->sk_err)
		err = -listener->sk_err;

	if (connected) {
		listener->sk_ack_backlog--;

		lock_sock(connected);
		vconnected = vsock_sk(connected);

		 
		if (err) {
			vconnected->rejected = true;
			release_sock(connected);
			sock_put(connected);
			goto out_wait;
		}

		newsock->state = SS_CONNECTED;
		sock_graft(connected, newsock);
		release_sock(connected);
		sock_put(connected);
	}

out_wait:
	finish_wait(sk_sleep(listener), &wait);
out:
	release_sock(listener);
	return err;
}
