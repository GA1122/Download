static int l2tp_ip_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
			   size_t len, int noblock, int flags, int *addr_len)
{
	struct inet_sock *inet = inet_sk(sk);
	struct l2tp_ip_sock *lsk = l2tp_ip_sk(sk);
	size_t copied = 0;
	int err = -EOPNOTSUPP;
	struct sockaddr_in *sin = (struct sockaddr_in *)msg->msg_name;
	struct sk_buff *skb;

	if (flags & MSG_OOB)
		goto out;

	if (addr_len)
		*addr_len = sizeof(*sin);

	skb = skb_recv_datagram(sk, flags, noblock, &err);
	if (!skb)
		goto out;

	copied = skb->len;
	if (len < copied) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
	}

	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (err)
		goto done;

	sock_recv_timestamp(msg, sk, skb);

	 
	if (sin) {
		sin->sin_family = AF_INET;
		sin->sin_addr.s_addr = ip_hdr(skb)->saddr;
		sin->sin_port = 0;
		memset(&sin->sin_zero, 0, sizeof(sin->sin_zero));
	}
	if (inet->cmsg_flags)
		ip_cmsg_recv(msg, skb);
	if (flags & MSG_TRUNC)
		copied = skb->len;
done:
	skb_free_datagram(sk, skb);
out:
	if (err) {
		lsk->rx_errors++;
		return err;
	}

	lsk->rx_packets++;
	lsk->rx_bytes += copied;

	return copied;
}