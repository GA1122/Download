static inline int l2cap_do_send(struct sock *sk, struct msghdr *msg, int len)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sk_buff *skb, **frag;
	int err, hlen, count, sent = 0;
	struct l2cap_hdr *lh;

	BT_DBG("sk %p len %d", sk, len);

	 
	if (sk->sk_type == SOCK_DGRAM)
		hlen = L2CAP_HDR_SIZE + 2;
	else
		hlen = L2CAP_HDR_SIZE;

	count = min_t(unsigned int, (conn->mtu - hlen), len);

	skb = bt_skb_send_alloc(sk, hlen + count,
			msg->msg_flags & MSG_DONTWAIT, &err);
	if (!skb)
		return err;

	 
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));

	if (sk->sk_type == SOCK_DGRAM)
		put_unaligned(l2cap_pi(sk)->psm, (__le16 *) skb_put(skb, 2));

	if (memcpy_fromiovec(skb_put(skb, count), msg->msg_iov, count)) {
		err = -EFAULT;
		goto fail;
	}

	sent += count;
	len  -= count;

	 
	frag = &skb_shinfo(skb)->frag_list;
	while (len) {
		count = min_t(unsigned int, conn->mtu, len);

		*frag = bt_skb_send_alloc(sk, count, msg->msg_flags & MSG_DONTWAIT, &err);
		if (!*frag)
			goto fail;

		if (memcpy_fromiovec(skb_put(*frag, count), msg->msg_iov, count)) {
			err = -EFAULT;
			goto fail;
		}

		sent += count;
		len  -= count;

		frag = &(*frag)->next;
	}
	err = hci_send_acl(conn->hcon, skb, 0);
	if (err < 0)
		goto fail;

	return sent;

fail:
	kfree_skb(skb);
	return err;
}