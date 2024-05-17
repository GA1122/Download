static int unix_stream_read_generic(struct unix_stream_read_state *state)
{
	struct scm_cookie scm;
	struct socket *sock = state->socket;
	struct sock *sk = sock->sk;
	struct unix_sock *u = unix_sk(sk);
	int copied = 0;
	int flags = state->flags;
	int noblock = flags & MSG_DONTWAIT;
	bool check_creds = false;
	int target;
	int err = 0;
	long timeo;
	int skip;
	size_t size = state->size;
	unsigned int last_len;

	err = -EINVAL;
	if (sk->sk_state != TCP_ESTABLISHED)
		goto out;

	err = -EOPNOTSUPP;
	if (flags & MSG_OOB)
		goto out;

	target = sock_rcvlowat(sk, flags & MSG_WAITALL, size);
	timeo = sock_rcvtimeo(sk, noblock);

	memset(&scm, 0, sizeof(scm));

	 
	err = mutex_lock_interruptible(&u->readlock);
	if (unlikely(err)) {
		 
		err = noblock ? -EAGAIN : -ERESTARTSYS;
		goto out;
	}

	if (flags & MSG_PEEK)
		skip = sk_peek_offset(sk, flags);
	else
		skip = 0;

	do {
		int chunk;
		bool drop_skb;
		struct sk_buff *skb, *last;

		unix_state_lock(sk);
		if (sock_flag(sk, SOCK_DEAD)) {
			err = -ECONNRESET;
			goto unlock;
		}
		last = skb = skb_peek(&sk->sk_receive_queue);
		last_len = last ? last->len : 0;
again:
		if (skb == NULL) {
			unix_sk(sk)->recursion_level = 0;
			if (copied >= target)
				goto unlock;

			 

			err = sock_error(sk);
			if (err)
				goto unlock;
			if (sk->sk_shutdown & RCV_SHUTDOWN)
				goto unlock;

			unix_state_unlock(sk);
			err = -EAGAIN;
			if (!timeo)
				break;
			mutex_unlock(&u->readlock);

			timeo = unix_stream_data_wait(sk, timeo, last,
						      last_len);

			if (signal_pending(current) ||
			    mutex_lock_interruptible(&u->readlock)) {
				err = sock_intr_errno(timeo);
				goto out;
			}

			continue;
unlock:
			unix_state_unlock(sk);
			break;
		}

		while (skip >= unix_skb_len(skb)) {
			skip -= unix_skb_len(skb);
			last = skb;
			last_len = skb->len;
			skb = skb_peek_next(skb, &sk->sk_receive_queue);
			if (!skb)
				goto again;
		}

		unix_state_unlock(sk);

		if (check_creds) {
			 
			if ((UNIXCB(skb).pid  != scm.pid) ||
			    !uid_eq(UNIXCB(skb).uid, scm.creds.uid) ||
			    !gid_eq(UNIXCB(skb).gid, scm.creds.gid) ||
			    !unix_secdata_eq(&scm, skb))
				break;
		} else if (test_bit(SOCK_PASSCRED, &sock->flags)) {
			 
			scm_set_cred(&scm, UNIXCB(skb).pid, UNIXCB(skb).uid, UNIXCB(skb).gid);
			unix_set_secdata(&scm, skb);
			check_creds = true;
		}

		 
		if (state->msg && state->msg->msg_name) {
			DECLARE_SOCKADDR(struct sockaddr_un *, sunaddr,
					 state->msg->msg_name);
			unix_copy_addr(state->msg, skb->sk);
			sunaddr = NULL;
		}

		chunk = min_t(unsigned int, unix_skb_len(skb) - skip, size);
		skb_get(skb);
		chunk = state->recv_actor(skb, skip, chunk, state);
		drop_skb = !unix_skb_len(skb);
		 
		consume_skb(skb);
		if (chunk < 0) {
			if (copied == 0)
				copied = -EFAULT;
			break;
		}
		copied += chunk;
		size -= chunk;

		if (drop_skb) {
			 
			err = 0;
			break;
		}

		 
		if (!(flags & MSG_PEEK)) {
			UNIXCB(skb).consumed += chunk;

			sk_peek_offset_bwd(sk, chunk);

			if (UNIXCB(skb).fp)
				unix_detach_fds(&scm, skb);

			if (unix_skb_len(skb))
				break;

			skb_unlink(skb, &sk->sk_receive_queue);
			consume_skb(skb);

			if (scm.fp)
				break;
		} else {
			 
			if (UNIXCB(skb).fp)
				scm.fp = scm_fp_dup(UNIXCB(skb).fp);

			sk_peek_offset_fwd(sk, chunk);

			if (UNIXCB(skb).fp)
				break;

			skip = 0;
			last = skb;
			last_len = skb->len;
			unix_state_lock(sk);
			skb = skb_peek_next(skb, &sk->sk_receive_queue);
			if (skb)
				goto again;
			unix_state_unlock(sk);
			break;
		}
	} while (size);

	mutex_unlock(&u->readlock);
	if (state->msg)
		scm_recv(sock, state->msg, &scm, flags);
	else
		scm_destroy(&scm);
out:
	return copied ? : err;
}