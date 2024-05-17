static int unix_stream_recvmsg(struct kiocb *iocb, struct socket *sock,
			       struct msghdr *msg, size_t size,
			       int flags)
{
	struct sock_iocb *siocb = kiocb_to_siocb(iocb);
	struct scm_cookie tmp_scm;
	struct sock *sk = sock->sk;
	struct unix_sock *u = unix_sk(sk);
	struct sockaddr_un *sunaddr = msg->msg_name;
	int copied = 0;
	int check_creds = 0;
	int target;
	int err = 0;
	long timeo;
	int skip;

	err = -EINVAL;
	if (sk->sk_state != TCP_ESTABLISHED)
		goto out;

	err = -EOPNOTSUPP;
	if (flags&MSG_OOB)
		goto out;

	target = sock_rcvlowat(sk, flags&MSG_WAITALL, size);
	timeo = sock_rcvtimeo(sk, flags&MSG_DONTWAIT);

	msg->msg_namelen = 0;

	 

	if (!siocb->scm) {
		siocb->scm = &tmp_scm;
		memset(&tmp_scm, 0, sizeof(tmp_scm));
	}

	err = mutex_lock_interruptible(&u->readlock);
	if (err) {
		err = sock_intr_errno(timeo);
		goto out;
	}

	skip = sk_peek_offset(sk, flags);

	do {
		int chunk;
		struct sk_buff *skb;

		unix_state_lock(sk);
		skb = skb_peek(&sk->sk_receive_queue);
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

			timeo = unix_stream_data_wait(sk, timeo);

			if (signal_pending(current)
			    ||  mutex_lock_interruptible(&u->readlock)) {
				err = sock_intr_errno(timeo);
				goto out;
			}

			continue;
 unlock:
			unix_state_unlock(sk);
			break;
		}

		if (skip >= skb->len) {
			skip -= skb->len;
			skb = skb_peek_next(skb, &sk->sk_receive_queue);
			goto again;
		}

		unix_state_unlock(sk);

		if (check_creds) {
			 
			if ((UNIXCB(skb).pid  != siocb->scm->pid) ||
			    (UNIXCB(skb).cred != siocb->scm->cred))
				break;
		} else {
			 
			scm_set_cred(siocb->scm, UNIXCB(skb).pid, UNIXCB(skb).cred);
			check_creds = 1;
		}

		 
		if (sunaddr) {
			unix_copy_addr(msg, skb->sk);
			sunaddr = NULL;
		}

		chunk = min_t(unsigned int, skb->len - skip, size);
		if (memcpy_toiovec(msg->msg_iov, skb->data + skip, chunk)) {
			if (copied == 0)
				copied = -EFAULT;
			break;
		}
		copied += chunk;
		size -= chunk;

		 
		if (!(flags & MSG_PEEK)) {
			skb_pull(skb, chunk);

			sk_peek_offset_bwd(sk, chunk);

			if (UNIXCB(skb).fp)
				unix_detach_fds(siocb->scm, skb);

			if (skb->len)
				break;

			skb_unlink(skb, &sk->sk_receive_queue);
			consume_skb(skb);

			if (siocb->scm->fp)
				break;
		} else {
			 
			if (UNIXCB(skb).fp)
				siocb->scm->fp = scm_fp_dup(UNIXCB(skb).fp);

			sk_peek_offset_fwd(sk, chunk);

			break;
		}
	} while (size);

	mutex_unlock(&u->readlock);
	scm_recv(sock, msg, siocb->scm, flags);
out:
	return copied ? : err;
}