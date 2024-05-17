SCTP_STATIC int sctp_do_bind(struct sock *sk, union sctp_addr *addr, int len)
{
	struct sctp_sock *sp = sctp_sk(sk);
	struct sctp_endpoint *ep = sp->ep;
	struct sctp_bind_addr *bp = &ep->base.bind_addr;
	struct sctp_af *af;
	unsigned short snum;
	int ret = 0;

	 
	af = sctp_sockaddr_af(sp, addr, len);
	if (!af) {
		SCTP_DEBUG_PRINTK("sctp_do_bind(sk: %p, newaddr: %p, len: %d) EINVAL\n",
				  sk, addr, len);
		return -EINVAL;
	}

	snum = ntohs(addr->v4.sin_port);

	SCTP_DEBUG_PRINTK_IPADDR("sctp_do_bind(sk: %p, new addr: ",
				 ", port: %d, new port: %d, len: %d)\n",
				 sk,
				 addr,
				 bp->port, snum,
				 len);

	 
	if (!sp->pf->bind_verify(sp, addr))
		return -EADDRNOTAVAIL;

	 
	if (bp->port && (snum != bp->port)) {
		SCTP_DEBUG_PRINTK("sctp_do_bind:"
				  " New port %d does not match existing port "
				  "%d.\n", snum, bp->port);
		return -EINVAL;
	}

	if (snum && snum < PROT_SOCK && !capable(CAP_NET_BIND_SERVICE))
		return -EACCES;

	 
	if ((ret = sctp_get_port_local(sk, addr))) {
		if (ret == (long) sk) {
			 
			return -EINVAL;
		} else {
			return -EADDRINUSE;
		}
	}

	 
	if (!bp->port)
		bp->port = inet_sk(sk)->num;

	 
	sctp_local_bh_disable();
	sctp_write_lock(&ep->base.addr_lock);

	 
	ret = sctp_add_bind_addr(bp, addr, 1, GFP_ATOMIC);
	sctp_write_unlock(&ep->base.addr_lock);
	sctp_local_bh_enable();

	 
	if (!ret) {
		inet_sk(sk)->sport = htons(inet_sk(sk)->num);
		af->to_sk_saddr(addr, sk);
	}

	return ret;
}