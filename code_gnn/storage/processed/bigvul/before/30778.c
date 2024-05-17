static int __must_check ax25_connect(struct socket *sock,
	struct sockaddr *uaddr, int addr_len, int flags)
{
	struct sock *sk = sock->sk;
	ax25_cb *ax25 = ax25_sk(sk), *ax25t;
	struct full_sockaddr_ax25 *fsa = (struct full_sockaddr_ax25 *)uaddr;
	ax25_digi *digi = NULL;
	int ct = 0, err = 0;

	 

	if (addr_len == sizeof(struct sockaddr_ax25))
		 
		;
	else if (addr_len != sizeof(struct full_sockaddr_ax25))
		 
		if ((addr_len < sizeof(struct sockaddr_ax25) + sizeof(ax25_address) * 6) ||
		    (addr_len > sizeof(struct full_sockaddr_ax25)))
			return -EINVAL;


	if (fsa->fsa_ax25.sax25_family != AF_AX25)
		return -EINVAL;

	lock_sock(sk);

	 
	if (sock->state == SS_CONNECTING) {
		switch (sk->sk_state) {
		case TCP_SYN_SENT:  
			err = -EINPROGRESS;
			goto out_release;

		case TCP_ESTABLISHED:  
			sock->state = SS_CONNECTED;
			goto out_release;

		case TCP_CLOSE:  
			sock->state = SS_UNCONNECTED;
			err = -ECONNREFUSED;
			goto out_release;
		}
	}

	if (sk->sk_state == TCP_ESTABLISHED && sk->sk_type == SOCK_SEQPACKET) {
		err = -EISCONN;	 
		goto out_release;
	}

	sk->sk_state   = TCP_CLOSE;
	sock->state = SS_UNCONNECTED;

	kfree(ax25->digipeat);
	ax25->digipeat = NULL;

	 
	if (addr_len > sizeof(struct sockaddr_ax25) &&
	    fsa->fsa_ax25.sax25_ndigis != 0) {
		 
		if (fsa->fsa_ax25.sax25_ndigis < 1 || fsa->fsa_ax25.sax25_ndigis > AX25_MAX_DIGIS) {
			err = -EINVAL;
			goto out_release;
		}

		if ((digi = kmalloc(sizeof(ax25_digi), GFP_KERNEL)) == NULL) {
			err = -ENOBUFS;
			goto out_release;
		}

		digi->ndigi      = fsa->fsa_ax25.sax25_ndigis;
		digi->lastrepeat = -1;

		while (ct < fsa->fsa_ax25.sax25_ndigis) {
			if ((fsa->fsa_digipeater[ct].ax25_call[6] &
			     AX25_HBIT) && ax25->iamdigi) {
				digi->repeated[ct] = 1;
				digi->lastrepeat   = ct;
			} else {
				digi->repeated[ct] = 0;
			}
			digi->calls[ct] = fsa->fsa_digipeater[ct];
			ct++;
		}
	}

	 
	if (sock_flag(sk, SOCK_ZAPPED)) {
		 
		printk(KERN_WARNING "ax25_connect(): %s uses autobind, please contact jreuter@yaina.de\n",
			current->comm);
		if ((err = ax25_rt_autobind(ax25, &fsa->fsa_ax25.sax25_call)) < 0) {
			kfree(digi);
			goto out_release;
		}

		ax25_fillin_cb(ax25, ax25->ax25_dev);
		ax25_cb_add(ax25);
	} else {
		if (ax25->ax25_dev == NULL) {
			kfree(digi);
			err = -EHOSTUNREACH;
			goto out_release;
		}
	}

	if (sk->sk_type == SOCK_SEQPACKET &&
	    (ax25t=ax25_find_cb(&ax25->source_addr, &fsa->fsa_ax25.sax25_call, digi,
			 ax25->ax25_dev->dev))) {
		kfree(digi);
		err = -EADDRINUSE;		 
		ax25_cb_put(ax25t);
		goto out_release;
	}

	ax25->dest_addr = fsa->fsa_ax25.sax25_call;
	ax25->digipeat  = digi;

	 
	if (sk->sk_type != SOCK_SEQPACKET) {
		sock->state = SS_CONNECTED;
		sk->sk_state   = TCP_ESTABLISHED;
		goto out_release;
	}

	 
	sock->state        = SS_CONNECTING;
	sk->sk_state          = TCP_SYN_SENT;

	switch (ax25->ax25_dev->values[AX25_VALUES_PROTOCOL]) {
	case AX25_PROTO_STD_SIMPLEX:
	case AX25_PROTO_STD_DUPLEX:
		ax25_std_establish_data_link(ax25);
		break;

#ifdef CONFIG_AX25_DAMA_SLAVE
	case AX25_PROTO_DAMA_SLAVE:
		ax25->modulus = AX25_MODULUS;
		ax25->window  = ax25->ax25_dev->values[AX25_VALUES_WINDOW];
		if (ax25->ax25_dev->dama.slave)
			ax25_ds_establish_data_link(ax25);
		else
			ax25_std_establish_data_link(ax25);
		break;
#endif
	}

	ax25->state = AX25_STATE_1;

	ax25_start_heartbeat(ax25);

	 
	if (sk->sk_state != TCP_ESTABLISHED && (flags & O_NONBLOCK)) {
		err = -EINPROGRESS;
		goto out_release;
	}

	if (sk->sk_state == TCP_SYN_SENT) {
		DEFINE_WAIT(wait);

		for (;;) {
			prepare_to_wait(sk_sleep(sk), &wait,
					TASK_INTERRUPTIBLE);
			if (sk->sk_state != TCP_SYN_SENT)
				break;
			if (!signal_pending(current)) {
				release_sock(sk);
				schedule();
				lock_sock(sk);
				continue;
			}
			err = -ERESTARTSYS;
			break;
		}
		finish_wait(sk_sleep(sk), &wait);

		if (err)
			goto out_release;
	}

	if (sk->sk_state != TCP_ESTABLISHED) {
		 
		sock->state = SS_UNCONNECTED;
		err = sock_error(sk);	 
		goto out_release;
	}

	sock->state = SS_CONNECTED;

	err = 0;
out_release:
	release_sock(sk);

	return err;
}
