static int irda_connect(struct socket *sock, struct sockaddr *uaddr,
			int addr_len, int flags)
{
	struct sock *sk = sock->sk;
	struct sockaddr_irda *addr = (struct sockaddr_irda *) uaddr;
	struct irda_sock *self = irda_sk(sk);
	int err;

	pr_debug("%s(%p)\n", __func__, self);

	lock_sock(sk);
	 
	err = -ESOCKTNOSUPPORT;
	if ((sk->sk_type == SOCK_DGRAM) && (sk->sk_protocol == IRDAPROTO_ULTRA))
		goto out;

	if (sk->sk_state == TCP_ESTABLISHED && sock->state == SS_CONNECTING) {
		sock->state = SS_CONNECTED;
		err = 0;
		goto out;    
	}

	if (sk->sk_state == TCP_CLOSE && sock->state == SS_CONNECTING) {
		sock->state = SS_UNCONNECTED;
		err = -ECONNREFUSED;
		goto out;
	}

	err = -EISCONN;       
	if (sk->sk_state == TCP_ESTABLISHED)
		goto out;

	sk->sk_state   = TCP_CLOSE;
	sock->state = SS_UNCONNECTED;

	err = -EINVAL;
	if (addr_len != sizeof(struct sockaddr_irda))
		goto out;

	 
	if ((!addr->sir_addr) || (addr->sir_addr == DEV_ADDR_ANY)) {
		 
		err = irda_discover_daddr_and_lsap_sel(self, addr->sir_name);
		if (err) {
			pr_debug("%s(), auto-connect failed!\n", __func__);
			goto out;
		}
	} else {
		 
		self->daddr = addr->sir_addr;
		pr_debug("%s(), daddr = %08x\n", __func__, self->daddr);

		 
		if((addr->sir_name[0] != '\0') ||
		   (addr->sir_lsap_sel >= 0x70)) {
			 
			err = irda_find_lsap_sel(self, addr->sir_name);
			if (err) {
				pr_debug("%s(), connect failed!\n", __func__);
				goto out;
			}
		} else {
			 
			self->dtsap_sel = addr->sir_lsap_sel;
		}
	}

	 
	if (!self->tsap)
		irda_open_tsap(self, LSAP_ANY, addr->sir_name);

	 
	sock->state = SS_CONNECTING;
	sk->sk_state   = TCP_SYN_SENT;

	 
	err = irttp_connect_request(self->tsap, self->dtsap_sel,
				    self->saddr, self->daddr, NULL,
				    self->max_sdu_size_rx, NULL);
	if (err) {
		pr_debug("%s(), connect failed!\n", __func__);
		goto out;
	}

	 
	err = -EINPROGRESS;
	if (sk->sk_state != TCP_ESTABLISHED && (flags & O_NONBLOCK))
		goto out;

	err = -ERESTARTSYS;
	if (wait_event_interruptible(*(sk_sleep(sk)),
				     (sk->sk_state != TCP_SYN_SENT)))
		goto out;

	if (sk->sk_state != TCP_ESTABLISHED) {
		sock->state = SS_UNCONNECTED;
		err = sock_error(sk);
		if (!err)
			err = -ECONNRESET;
		goto out;
	}

	sock->state = SS_CONNECTED;

	 
	self->saddr = irttp_get_saddr(self->tsap);
	err = 0;
out:
	release_sock(sk);
	return err;
}
