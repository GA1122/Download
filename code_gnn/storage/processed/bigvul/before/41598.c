static int irda_sendmsg_dgram(struct socket *sock, struct msghdr *msg,
			      size_t len)
{
	struct sock *sk = sock->sk;
	struct irda_sock *self;
	struct sk_buff *skb;
	int err;

	pr_debug("%s(), len=%zd\n", __func__, len);

	if (msg->msg_flags & ~(MSG_DONTWAIT|MSG_CMSG_COMPAT))
		return -EINVAL;

	lock_sock(sk);

	if (sk->sk_shutdown & SEND_SHUTDOWN) {
		send_sig(SIGPIPE, current, 0);
		err = -EPIPE;
		goto out;
	}

	err = -ENOTCONN;
	if (sk->sk_state != TCP_ESTABLISHED)
		goto out;

	self = irda_sk(sk);

	 
	if (len > self->max_data_size) {
		pr_debug("%s(), Warning too much data! Chopping frame from %zd to %d bytes!\n",
			 __func__, len, self->max_data_size);
		len = self->max_data_size;
	}

	skb = sock_alloc_send_skb(sk, len + self->max_header_size,
				  msg->msg_flags & MSG_DONTWAIT, &err);
	err = -ENOBUFS;
	if (!skb)
		goto out;

	skb_reserve(skb, self->max_header_size);
	skb_reset_transport_header(skb);

	pr_debug("%s(), appending user data\n", __func__);
	skb_put(skb, len);
	err = memcpy_from_msg(skb_transport_header(skb), msg, len);
	if (err) {
		kfree_skb(skb);
		goto out;
	}

	 
	err = irttp_udata_request(self->tsap, skb);
	if (err) {
		pr_debug("%s(), err=%d\n", __func__, err);
		goto out;
	}

	release_sock(sk);
	return len;

out:
	release_sock(sk);
	return err;
}