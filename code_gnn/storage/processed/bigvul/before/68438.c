static int sctp_do_bind(struct sock *sk, union sctp_addr *addr, int len)
{
	struct net *net = sock_net(sk);
	struct sctp_sock *sp = sctp_sk(sk);
	struct sctp_endpoint *ep = sp->ep;
	struct sctp_bind_addr *bp = &ep->base.bind_addr;
	struct sctp_af *af;
	unsigned short snum;
	int ret = 0;

	 
	af = sctp_sockaddr_af(sp, addr, len);
	if (!af) {
		pr_debug("%s: sk:%p, newaddr:%p, len:%d EINVAL\n",
			 __func__, sk, addr, len);
		return -EINVAL;
	}

	snum = ntohs(addr->v4.sin_port);

	pr_debug("%s: sk:%p, new addr:%pISc, port:%d, new port:%d, len:%d\n",
		 __func__, sk, &addr->sa, bp->port, snum, len);

	 
	if (!sp->pf->bind_verify(sp, addr))
		return -EADDRNOTAVAIL;

	 
	if (bp->port) {
		if (!snum)
			snum = bp->port;
		else if (snum != bp->port) {
			pr_debug("%s: new port %d doesn't match existing port "
				 "%d\n", __func__, snum, bp->port);
			return -EINVAL;
		}
	}

	if (snum && snum < PROT_SOCK &&
	    !ns_capable(net->user_ns, CAP_NET_BIND_SERVICE))
		return -EACCES;

	 
	if (sctp_bind_addr_match(bp, addr, sp))
		return -EINVAL;

	 
	addr->v4.sin_port = htons(snum);
	if ((ret = sctp_get_port_local(sk, addr))) {
		return -EADDRINUSE;
	}

	 
	if (!bp->port)
		bp->port = inet_sk(sk)->inet_num;

	 
	ret = sctp_add_bind_addr(bp, addr, af->sockaddr_len,
				 SCTP_ADDR_SRC, GFP_ATOMIC);

	 
	if (!ret) {
		inet_sk(sk)->inet_sport = htons(inet_sk(sk)->inet_num);
		sp->pf->to_sk_saddr(addr, sk);
	}

	return ret;
}
