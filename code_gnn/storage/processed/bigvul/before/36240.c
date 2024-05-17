struct sctp_transport *sctp_assoc_add_peer(struct sctp_association *asoc,
					   const union sctp_addr *addr,
					   const gfp_t gfp,
					   const int peer_state)
{
	struct net *net = sock_net(asoc->base.sk);
	struct sctp_transport *peer;
	struct sctp_sock *sp;
	unsigned short port;

	sp = sctp_sk(asoc->base.sk);

	 
	port = ntohs(addr->v4.sin_port);

	pr_debug("%s: association:%p addr:%pISpc state:%d\n", __func__,
		 asoc, &addr->sa, peer_state);

	 
	if (0 == asoc->peer.port)
		asoc->peer.port = port;

	 
	peer = sctp_assoc_lookup_paddr(asoc, addr);
	if (peer) {
		 
		if (peer->state == SCTP_UNKNOWN) {
			peer->state = SCTP_ACTIVE;
		}
		return peer;
	}

	peer = sctp_transport_new(net, addr, gfp);
	if (!peer)
		return NULL;

	sctp_transport_set_owner(peer, asoc);

	 
	peer->hbinterval = asoc->hbinterval;

	 
	peer->pathmaxrxt = asoc->pathmaxrxt;

	 
	peer->pf_retrans = asoc->pf_retrans;

	 
	peer->sackdelay = asoc->sackdelay;
	peer->sackfreq = asoc->sackfreq;

	 
	peer->param_flags = asoc->param_flags;

	sctp_transport_route(peer, NULL, sp);

	 
	if (peer->param_flags & SPP_PMTUD_DISABLE) {
		if (asoc->pathmtu)
			peer->pathmtu = asoc->pathmtu;
		else
			peer->pathmtu = SCTP_DEFAULT_MAXSEGMENT;
	}

	 
	if (asoc->pathmtu)
		asoc->pathmtu = min_t(int, peer->pathmtu, asoc->pathmtu);
	else
		asoc->pathmtu = peer->pathmtu;

	pr_debug("%s: association:%p PMTU set to %d\n", __func__, asoc,
		 asoc->pathmtu);

	peer->pmtu_pending = 0;

	asoc->frag_point = sctp_frag_point(asoc, asoc->pathmtu);

	 
	sctp_packet_init(&peer->packet, peer, asoc->base.bind_addr.port,
			 asoc->peer.port);

	 
	peer->cwnd = min(4*asoc->pathmtu, max_t(__u32, 2*asoc->pathmtu, 4380));

	 
	peer->ssthresh = SCTP_DEFAULT_MAXWINDOW;

	peer->partial_bytes_acked = 0;
	peer->flight_size = 0;
	peer->burst_limited = 0;

	 
	peer->rto = asoc->rto_initial;
	sctp_max_rto(asoc, peer);

	 
	peer->state = peer_state;

	 
	list_add_tail_rcu(&peer->transports, &asoc->peer.transport_addr_list);
	asoc->peer.transport_count++;

	 
	if (!asoc->peer.primary_path) {
		sctp_assoc_set_primary(asoc, peer);
		asoc->peer.retran_path = peer;
	}

	if (asoc->peer.active_path == asoc->peer.retran_path &&
	    peer->state != SCTP_UNCONFIRMED) {
		asoc->peer.retran_path = peer;
	}

	return peer;
}
