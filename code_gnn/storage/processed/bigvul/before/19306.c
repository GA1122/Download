static int unix_listen(struct socket *sock, int backlog)
{
	int err;
	struct sock *sk = sock->sk;
	struct unix_sock *u = unix_sk(sk);
	struct pid *old_pid = NULL;
	const struct cred *old_cred = NULL;

	err = -EOPNOTSUPP;
	if (sock->type != SOCK_STREAM && sock->type != SOCK_SEQPACKET)
		goto out;	 
	err = -EINVAL;
	if (!u->addr)
		goto out;	 
	unix_state_lock(sk);
	if (sk->sk_state != TCP_CLOSE && sk->sk_state != TCP_LISTEN)
		goto out_unlock;
	if (backlog > sk->sk_max_ack_backlog)
		wake_up_interruptible_all(&u->peer_wait);
	sk->sk_max_ack_backlog	= backlog;
	sk->sk_state		= TCP_LISTEN;
	 
	init_peercred(sk);
	err = 0;

out_unlock:
	unix_state_unlock(sk);
	put_pid(old_pid);
	if (old_cred)
		put_cred(old_cred);
out:
	return err;
}