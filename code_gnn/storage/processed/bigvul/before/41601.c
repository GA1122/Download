static int irda_shutdown(struct socket *sock, int how)
{
	struct sock *sk = sock->sk;
	struct irda_sock *self = irda_sk(sk);

	pr_debug("%s(%p)\n", __func__, self);

	lock_sock(sk);

	sk->sk_state       = TCP_CLOSE;
	sk->sk_shutdown   |= SEND_SHUTDOWN;
	sk->sk_state_change(sk);

	if (self->iriap) {
		iriap_close(self->iriap);
		self->iriap = NULL;
	}

	if (self->tsap) {
		irttp_disconnect_request(self->tsap, NULL, P_NORMAL);
		irttp_close_tsap(self->tsap);
		self->tsap = NULL;
	}

	 
	self->rx_flow = self->tx_flow = FLOW_START;	 
	self->daddr = DEV_ADDR_ANY;	 
	self->saddr = 0x0;		 

	release_sock(sk);

	return 0;
}