static int caif_stream_sendmsg(struct kiocb *kiocb, struct socket *sock,
				struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct caifsock *cf_sk = container_of(sk, struct caifsock, sk);
	int err, size;
	struct sk_buff *skb;
	int sent = 0;
	long timeo;

	err = -EOPNOTSUPP;
	if (unlikely(msg->msg_flags&MSG_OOB))
		goto out_err;

	if (unlikely(msg->msg_namelen))
		goto out_err;

	timeo = sock_sndtimeo(sk, msg->msg_flags & MSG_DONTWAIT);
	timeo = caif_wait_for_flow_on(cf_sk, 1, timeo, &err);

	if (unlikely(sk->sk_shutdown & SEND_SHUTDOWN))
		goto pipe_err;

	while (sent < len) {

		size = len-sent;

		if (size > cf_sk->maxframe)
			size = cf_sk->maxframe;

		 
		if (size > ((sk->sk_sndbuf >> 1) - 64))
			size = (sk->sk_sndbuf >> 1) - 64;

		if (size > SKB_MAX_ALLOC)
			size = SKB_MAX_ALLOC;

		skb = sock_alloc_send_skb(sk,
					size + cf_sk->headroom +
					cf_sk->tailroom,
					msg->msg_flags&MSG_DONTWAIT,
					&err);
		if (skb == NULL)
			goto out_err;

		skb_reserve(skb, cf_sk->headroom);
		 
		size = min_t(int, size, skb_tailroom(skb));

		err = memcpy_fromiovec(skb_put(skb, size), msg->msg_iov, size);
		if (err) {
			kfree_skb(skb);
			goto out_err;
		}
		err = transmit_skb(skb, cf_sk,
				msg->msg_flags&MSG_DONTWAIT, timeo);
		if (err < 0)
			 
			goto pipe_err;

		sent += size;
	}

	return sent;

pipe_err:
	if (sent == 0 && !(msg->msg_flags&MSG_NOSIGNAL))
		send_sig(SIGPIPE, current, 0);
	err = -EPIPE;
out_err:
	return sent ? : err;
}
