static ssize_t unix_stream_sendpage(struct socket *socket, struct page *page,
				    int offset, size_t size, int flags)
{
	int err;
	bool send_sigpipe = false;
	bool init_scm = true;
	struct scm_cookie scm;
	struct sock *other, *sk = socket->sk;
	struct sk_buff *skb, *newskb = NULL, *tail = NULL;

	if (flags & MSG_OOB)
		return -EOPNOTSUPP;

	other = unix_peer(sk);
	if (!other || sk->sk_state != TCP_ESTABLISHED)
		return -ENOTCONN;

	if (false) {
alloc_skb:
		unix_state_unlock(other);
		mutex_unlock(&unix_sk(other)->readlock);
		newskb = sock_alloc_send_pskb(sk, 0, 0, flags & MSG_DONTWAIT,
					      &err, 0);
		if (!newskb)
			goto err;
	}

	 
	err = mutex_lock_interruptible(&unix_sk(other)->readlock);
	if (err) {
		err = flags & MSG_DONTWAIT ? -EAGAIN : -ERESTARTSYS;
		goto err;
	}

	if (sk->sk_shutdown & SEND_SHUTDOWN) {
		err = -EPIPE;
		send_sigpipe = true;
		goto err_unlock;
	}

	unix_state_lock(other);

	if (sock_flag(other, SOCK_DEAD) ||
	    other->sk_shutdown & RCV_SHUTDOWN) {
		err = -EPIPE;
		send_sigpipe = true;
		goto err_state_unlock;
	}

	if (init_scm) {
		err = maybe_init_creds(&scm, socket, other);
		if (err)
			goto err_state_unlock;
		init_scm = false;
	}

	skb = skb_peek_tail(&other->sk_receive_queue);
	if (tail && tail == skb) {
		skb = newskb;
	} else if (!skb || !unix_skb_scm_eq(skb, &scm)) {
		if (newskb) {
			skb = newskb;
		} else {
			tail = skb;
			goto alloc_skb;
		}
	} else if (newskb) {
		 
		consume_skb(newskb);
		newskb = NULL;
	}

	if (skb_append_pagefrags(skb, page, offset, size)) {
		tail = skb;
		goto alloc_skb;
	}

	skb->len += size;
	skb->data_len += size;
	skb->truesize += size;
	atomic_add(size, &sk->sk_wmem_alloc);

	if (newskb) {
		err = unix_scm_to_skb(&scm, skb, false);
		if (err)
			goto err_state_unlock;
		spin_lock(&other->sk_receive_queue.lock);
		__skb_queue_tail(&other->sk_receive_queue, newskb);
		spin_unlock(&other->sk_receive_queue.lock);
	}

	unix_state_unlock(other);
	mutex_unlock(&unix_sk(other)->readlock);

	other->sk_data_ready(other);
	scm_destroy(&scm);
	return size;

err_state_unlock:
	unix_state_unlock(other);
err_unlock:
	mutex_unlock(&unix_sk(other)->readlock);
err:
	kfree_skb(newskb);
	if (send_sigpipe && !(flags & MSG_NOSIGNAL))
		send_sig(SIGPIPE, current, 0);
	if (!init_scm)
		scm_destroy(&scm);
	return err;
}