static int irda_accept(struct socket *sock, struct socket *newsock, int flags)
{
	struct sock *sk = sock->sk;
	struct irda_sock *new, *self = irda_sk(sk);
	struct sock *newsk;
	struct sk_buff *skb;
	int err;

	IRDA_DEBUG(2, "%s()\n", __func__);

	err = irda_create(sock_net(sk), newsock, sk->sk_protocol, 0);
	if (err)
		return err;

	err = -EINVAL;

	lock_sock(sk);
	if (sock->state != SS_UNCONNECTED)
		goto out;

	if ((sk = sock->sk) == NULL)
		goto out;

	err = -EOPNOTSUPP;
	if ((sk->sk_type != SOCK_STREAM) && (sk->sk_type != SOCK_SEQPACKET) &&
	    (sk->sk_type != SOCK_DGRAM))
		goto out;

	err = -EINVAL;
	if (sk->sk_state != TCP_LISTEN)
		goto out;

	 

	 
	while (1) {
		skb = skb_dequeue(&sk->sk_receive_queue);
		if (skb)
			break;

		 
		err = -EWOULDBLOCK;
		if (flags & O_NONBLOCK)
			goto out;

		err = wait_event_interruptible(*(sk_sleep(sk)),
					skb_peek(&sk->sk_receive_queue));
		if (err)
			goto out;
	}

	newsk = newsock->sk;
	err = -EIO;
	if (newsk == NULL)
		goto out;

	newsk->sk_state = TCP_ESTABLISHED;

	new = irda_sk(newsk);

	 
	new->tsap = irttp_dup(self->tsap, new);
	err = -EPERM;  
	if (!new->tsap) {
		IRDA_DEBUG(0, "%s(), dup failed!\n", __func__);
		kfree_skb(skb);
		goto out;
	}

	new->stsap_sel = new->tsap->stsap_sel;
	new->dtsap_sel = new->tsap->dtsap_sel;
	new->saddr = irttp_get_saddr(new->tsap);
	new->daddr = irttp_get_daddr(new->tsap);

	new->max_sdu_size_tx = self->max_sdu_size_tx;
	new->max_sdu_size_rx = self->max_sdu_size_rx;
	new->max_data_size   = self->max_data_size;
	new->max_header_size = self->max_header_size;

	memcpy(&new->qos_tx, &self->qos_tx, sizeof(struct qos_info));

	 
	irttp_listen(self->tsap);

	kfree_skb(skb);
	sk->sk_ack_backlog--;

	newsock->state = SS_CONNECTED;

	irda_connect_response(new);
	err = 0;
out:
	release_sock(sk);
	return err;
}
