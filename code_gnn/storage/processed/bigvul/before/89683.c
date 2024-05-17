int nfc_llcp_send_ui_frame(struct nfc_llcp_sock *sock, u8 ssap, u8 dsap,
			   struct msghdr *msg, size_t len)
{
	struct sk_buff *pdu;
	struct nfc_llcp_local *local;
	size_t frag_len = 0, remaining_len;
	u8 *msg_ptr, *msg_data;
	u16 remote_miu;
	int err;

	pr_debug("Send UI frame len %zd\n", len);

	local = sock->local;
	if (local == NULL)
		return -ENODEV;

	msg_data = kmalloc(len, GFP_USER | __GFP_NOWARN);
	if (msg_data == NULL)
		return -ENOMEM;

	if (memcpy_from_msg(msg_data, msg, len)) {
		kfree(msg_data);
		return -EFAULT;
	}

	remaining_len = len;
	msg_ptr = msg_data;

	do {
		remote_miu = sock->remote_miu > LLCP_MAX_MIU ?
				local->remote_miu : sock->remote_miu;

		frag_len = min_t(size_t, remote_miu, remaining_len);

		pr_debug("Fragment %zd bytes remaining %zd",
			 frag_len, remaining_len);

		pdu = nfc_alloc_send_skb(sock->dev, &sock->sk, 0,
					 frag_len + LLCP_HEADER_SIZE, &err);
		if (pdu == NULL) {
			pr_err("Could not allocate PDU (error=%d)\n", err);
			len -= remaining_len;
			if (len == 0)
				len = err;
			break;
		}

		pdu = llcp_add_header(pdu, dsap, ssap, LLCP_PDU_UI);

		if (likely(frag_len > 0))
			skb_put_data(pdu, msg_ptr, frag_len);

		 
		skb_queue_tail(&local->tx_queue, pdu);

		remaining_len -= frag_len;
		msg_ptr += frag_len;
	} while (remaining_len > 0);

	kfree(msg_data);

	return len;
}