static int dn_recvmsg(struct socket *sock, struct msghdr *msg, size_t size,
		      int flags)
{
	struct sock *sk = sock->sk;
	struct dn_scp *scp = DN_SK(sk);
	struct sk_buff_head *queue = &sk->sk_receive_queue;
	size_t target = size > 1 ? 1 : 0;
	size_t copied = 0;
	int rv = 0;
	struct sk_buff *skb, *n;
	struct dn_skb_cb *cb = NULL;
	unsigned char eor = 0;
	long timeo = sock_rcvtimeo(sk, flags & MSG_DONTWAIT);

	lock_sock(sk);

	if (sock_flag(sk, SOCK_ZAPPED)) {
		rv = -EADDRNOTAVAIL;
		goto out;
	}

	if (sk->sk_shutdown & RCV_SHUTDOWN) {
		rv = 0;
		goto out;
	}

	rv = dn_check_state(sk, NULL, 0, &timeo, flags);
	if (rv)
		goto out;

	if (flags & ~(MSG_CMSG_COMPAT|MSG_PEEK|MSG_OOB|MSG_WAITALL|MSG_DONTWAIT|MSG_NOSIGNAL)) {
		rv = -EOPNOTSUPP;
		goto out;
	}

	if (flags & MSG_OOB)
		queue = &scp->other_receive_queue;

	if (flags & MSG_WAITALL)
		target = size;


	 
	for(;;) {
		DEFINE_WAIT(wait);

		if (sk->sk_err)
			goto out;

		if (!skb_queue_empty(&scp->other_receive_queue)) {
			if (!(flags & MSG_OOB)) {
				msg->msg_flags |= MSG_OOB;
				if (!scp->other_report) {
					scp->other_report = 1;
					goto out;
				}
			}
		}

		if (scp->state != DN_RUN)
			goto out;

		if (signal_pending(current)) {
			rv = sock_intr_errno(timeo);
			goto out;
		}

		if (dn_data_ready(sk, queue, flags, target))
			break;

		if (flags & MSG_DONTWAIT) {
			rv = -EWOULDBLOCK;
			goto out;
		}

		prepare_to_wait(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);
		sk_set_bit(SOCKWQ_ASYNC_WAITDATA, sk);
		sk_wait_event(sk, &timeo, dn_data_ready(sk, queue, flags, target));
		sk_clear_bit(SOCKWQ_ASYNC_WAITDATA, sk);
		finish_wait(sk_sleep(sk), &wait);
	}

	skb_queue_walk_safe(queue, skb, n) {
		unsigned int chunk = skb->len;
		cb = DN_SKB_CB(skb);

		if ((chunk + copied) > size)
			chunk = size - copied;

		if (memcpy_to_msg(msg, skb->data, chunk)) {
			rv = -EFAULT;
			break;
		}
		copied += chunk;

		if (!(flags & MSG_PEEK))
			skb_pull(skb, chunk);

		eor = cb->nsp_flags & 0x40;

		if (skb->len == 0) {
			skb_unlink(skb, queue);
			kfree_skb(skb);
			 
			if ((scp->flowloc_sw == DN_DONTSEND) && !dn_congested(sk)) {
				scp->flowloc_sw = DN_SEND;
				dn_nsp_send_link(sk, DN_SEND, 0);
			}
		}

		if (eor) {
			if (sk->sk_type == SOCK_SEQPACKET)
				break;
			if (!(flags & MSG_WAITALL))
				break;
		}

		if (flags & MSG_OOB)
			break;

		if (copied >= target)
			break;
	}

	rv = copied;


	if (eor && (sk->sk_type == SOCK_SEQPACKET))
		msg->msg_flags |= MSG_EOR;

out:
	if (rv == 0)
		rv = (flags & MSG_PEEK) ? -sk->sk_err : sock_error(sk);

	if ((rv >= 0) && msg->msg_name) {
		__sockaddr_check_size(sizeof(struct sockaddr_dn));
		memcpy(msg->msg_name, &scp->peer, sizeof(struct sockaddr_dn));
		msg->msg_namelen = sizeof(struct sockaddr_dn);
	}

	release_sock(sk);

	return rv;
}