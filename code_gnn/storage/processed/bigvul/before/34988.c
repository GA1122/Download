static int __sctp_connect(struct sock* sk,
			  struct sockaddr *kaddrs,
			  int addrs_size)
{
	struct sctp_sock *sp;
	struct sctp_endpoint *ep;
	struct sctp_association *asoc = NULL;
	struct sctp_association *asoc2;
	struct sctp_transport *transport;
	union sctp_addr to;
	struct sctp_af *af;
	sctp_scope_t scope;
	long timeo;
	int err = 0;
	int addrcnt = 0;
	int walk_size = 0;
	union sctp_addr *sa_addr;
	void *addr_buf;

	sp = sctp_sk(sk);
	ep = sp->ep;

	 
	if (sctp_sstate(sk, ESTABLISHED) ||
	    (sctp_style(sk, TCP) && sctp_sstate(sk, LISTENING))) {
		err = -EISCONN;
		goto out_free;
	}

	 
	addr_buf = kaddrs;
	while (walk_size < addrs_size) {
		sa_addr = (union sctp_addr *)addr_buf;
		af = sctp_get_af_specific(sa_addr->sa.sa_family);

		 
		if (!af || (walk_size + af->sockaddr_len) > addrs_size) {
			err = -EINVAL;
			goto out_free;
		}

		err = sctp_verify_addr(sk, sa_addr, af->sockaddr_len);
		if (err)
			goto out_free;

		memcpy(&to, sa_addr, af->sockaddr_len);

		 
		asoc2 = sctp_endpoint_lookup_assoc(ep, sa_addr, &transport);
		if (asoc2 && asoc2 != asoc) {
			if (asoc2->state >= SCTP_STATE_ESTABLISHED)
				err = -EISCONN;
			else
				err = -EALREADY;
			goto out_free;
		}

		 
		if (sctp_endpoint_is_peeled_off(ep, sa_addr)) {
			err = -EADDRNOTAVAIL;
			goto out_free;
		}

		if (!asoc) {
			 
			if (!ep->base.bind_addr.port) {
				if (sctp_autobind(sk)) {
					err = -EAGAIN;
					goto out_free;
				}
			} else {
				 
				if (ep->base.bind_addr.port < PROT_SOCK &&
				    !capable(CAP_NET_BIND_SERVICE)) {
					err = -EACCES;
					goto out_free;
				}
			}

			scope = sctp_scope(sa_addr);
			asoc = sctp_association_new(ep, sk, scope, GFP_KERNEL);
			if (!asoc) {
				err = -ENOMEM;
				goto out_free;
			}
		}

		 
		transport = sctp_assoc_add_peer(asoc, sa_addr, GFP_KERNEL,
						SCTP_UNKNOWN);
		if (!transport) {
			err = -ENOMEM;
			goto out_free;
		}

		addrcnt++;
		addr_buf += af->sockaddr_len;
		walk_size += af->sockaddr_len;
	}

	err = sctp_assoc_set_bind_addr_from_ep(asoc, GFP_KERNEL);
	if (err < 0) {
		goto out_free;
	}

	err = sctp_primitive_ASSOCIATE(asoc, NULL);
	if (err < 0) {
		goto out_free;
	}

	 
	inet_sk(sk)->dport = htons(asoc->peer.port);
	af = sctp_get_af_specific(to.sa.sa_family);
	af->to_sk_daddr(&to, sk);
	sk->sk_err = 0;

	timeo = sock_sndtimeo(sk, sk->sk_socket->file->f_flags & O_NONBLOCK);
	err = sctp_wait_for_connect(asoc, &timeo);

	 
	asoc = NULL;

out_free:

	SCTP_DEBUG_PRINTK("About to exit __sctp_connect() free asoc: %p"
			  " kaddrs: %p err: %d\n",
			  asoc, kaddrs, err);
	if (asoc)
		sctp_association_free(asoc);
	return err;
}
