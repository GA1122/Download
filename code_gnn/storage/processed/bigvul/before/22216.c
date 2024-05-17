static int rose_sendmsg(struct kiocb *iocb, struct socket *sock,
			struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct rose_sock *rose = rose_sk(sk);
	struct sockaddr_rose *usrose = (struct sockaddr_rose *)msg->msg_name;
	int err;
	struct full_sockaddr_rose srose;
	struct sk_buff *skb;
	unsigned char *asmptr;
	int n, size, qbit = 0;

	if (msg->msg_flags & ~(MSG_DONTWAIT|MSG_EOR|MSG_CMSG_COMPAT))
		return -EINVAL;

	if (sock_flag(sk, SOCK_ZAPPED))
		return -EADDRNOTAVAIL;

	if (sk->sk_shutdown & SEND_SHUTDOWN) {
		send_sig(SIGPIPE, current, 0);
		return -EPIPE;
	}

	if (rose->neighbour == NULL || rose->device == NULL)
		return -ENETUNREACH;

	if (usrose != NULL) {
		if (msg->msg_namelen != sizeof(struct sockaddr_rose) && msg->msg_namelen != sizeof(struct full_sockaddr_rose))
			return -EINVAL;
		memset(&srose, 0, sizeof(struct full_sockaddr_rose));
		memcpy(&srose, usrose, msg->msg_namelen);
		if (rosecmp(&rose->dest_addr, &srose.srose_addr) != 0 ||
		    ax25cmp(&rose->dest_call, &srose.srose_call) != 0)
			return -EISCONN;
		if (srose.srose_ndigis != rose->dest_ndigis)
			return -EISCONN;
		if (srose.srose_ndigis == rose->dest_ndigis) {
			for (n = 0 ; n < srose.srose_ndigis ; n++)
				if (ax25cmp(&rose->dest_digis[n],
					    &srose.srose_digis[n]))
					return -EISCONN;
		}
		if (srose.srose_family != AF_ROSE)
			return -EINVAL;
	} else {
		if (sk->sk_state != TCP_ESTABLISHED)
			return -ENOTCONN;

		srose.srose_family = AF_ROSE;
		srose.srose_addr   = rose->dest_addr;
		srose.srose_call   = rose->dest_call;
		srose.srose_ndigis = rose->dest_ndigis;
		for (n = 0 ; n < rose->dest_ndigis ; n++)
			srose.srose_digis[n] = rose->dest_digis[n];
	}

	SOCK_DEBUG(sk, "ROSE: sendto: Addresses built.\n");

	 
	SOCK_DEBUG(sk, "ROSE: sendto: building packet.\n");
	 
	if (len > 65535)
		return -EMSGSIZE;

	size = len + AX25_BPQ_HEADER_LEN + AX25_MAX_HEADER_LEN + ROSE_MIN_LEN;

	if ((skb = sock_alloc_send_skb(sk, size, msg->msg_flags & MSG_DONTWAIT, &err)) == NULL)
		return err;

	skb_reserve(skb, AX25_BPQ_HEADER_LEN + AX25_MAX_HEADER_LEN + ROSE_MIN_LEN);

	 
	SOCK_DEBUG(sk, "ROSE: Appending user data\n");

	skb_reset_transport_header(skb);
	skb_put(skb, len);

	err = memcpy_fromiovec(skb_transport_header(skb), msg->msg_iov, len);
	if (err) {
		kfree_skb(skb);
		return err;
	}

	 
	if (rose->qbitincl) {
		qbit = skb->data[0];
		skb_pull(skb, 1);
	}

	 
	asmptr = skb_push(skb, ROSE_MIN_LEN);

	SOCK_DEBUG(sk, "ROSE: Building Network Header.\n");

	 
	asmptr[0] = ((rose->lci >> 8) & 0x0F) | ROSE_GFI;
	asmptr[1] = (rose->lci >> 0) & 0xFF;
	asmptr[2] = ROSE_DATA;

	if (qbit)
		asmptr[0] |= ROSE_Q_BIT;

	SOCK_DEBUG(sk, "ROSE: Built header.\n");

	SOCK_DEBUG(sk, "ROSE: Transmitting buffer\n");

	if (sk->sk_state != TCP_ESTABLISHED) {
		kfree_skb(skb);
		return -ENOTCONN;
	}

#ifdef M_BIT
#define ROSE_PACLEN (256-ROSE_MIN_LEN)
	if (skb->len - ROSE_MIN_LEN > ROSE_PACLEN) {
		unsigned char header[ROSE_MIN_LEN];
		struct sk_buff *skbn;
		int frontlen;
		int lg;

		 
		skb_copy_from_linear_data(skb, header, ROSE_MIN_LEN);
		skb_pull(skb, ROSE_MIN_LEN);

		frontlen = skb_headroom(skb);

		while (skb->len > 0) {
			if ((skbn = sock_alloc_send_skb(sk, frontlen + ROSE_PACLEN, 0, &err)) == NULL) {
				kfree_skb(skb);
				return err;
			}

			skbn->sk   = sk;
			skbn->free = 1;
			skbn->arp  = 1;

			skb_reserve(skbn, frontlen);

			lg = (ROSE_PACLEN > skb->len) ? skb->len : ROSE_PACLEN;

			 
			skb_copy_from_linear_data(skb, skb_put(skbn, lg), lg);
			skb_pull(skb, lg);

			 
			skb_push(skbn, ROSE_MIN_LEN);
			skb_copy_to_linear_data(skbn, header, ROSE_MIN_LEN);

			if (skb->len > 0)
				skbn->data[2] |= M_BIT;

			skb_queue_tail(&sk->sk_write_queue, skbn);  
		}

		skb->free = 1;
		kfree_skb(skb);
	} else {
		skb_queue_tail(&sk->sk_write_queue, skb);		 
	}
#else
	skb_queue_tail(&sk->sk_write_queue, skb);	 
#endif

	rose_kick(sk);

	return len;
}