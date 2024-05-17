void vsock_pending_work(struct work_struct *work)
{
	struct sock *sk;
	struct sock *listener;
	struct vsock_sock *vsk;
	bool cleanup;

	vsk = container_of(work, struct vsock_sock, dwork.work);
	sk = sk_vsock(vsk);
	listener = vsk->listener;
	cleanup = true;

	lock_sock(listener);
	lock_sock(sk);

	if (vsock_is_pending(sk)) {
		vsock_remove_pending(listener, sk);
	} else if (!vsk->rejected) {
		 
		cleanup = false;
		goto out;
	}

	listener->sk_ack_backlog--;

	 
	if (vsock_in_connected_table(vsk))
		vsock_remove_connected(vsk);

	sk->sk_state = SS_FREE;

out:
	release_sock(sk);
	release_sock(listener);
	if (cleanup)
		sock_put(sk);

	sock_put(sk);
	sock_put(listener);
}
