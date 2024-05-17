static int sctp_process_param(struct sctp_association *asoc,
			      union sctp_params param,
			      const union sctp_addr *peer_addr,
			      gfp_t gfp)
{
	struct net *net = sock_net(asoc->base.sk);
	union sctp_addr addr;
	int i;
	__u16 sat;
	int retval = 1;
	sctp_scope_t scope;
	time_t stale;
	struct sctp_af *af;
	union sctp_addr_param *addr_param;
	struct sctp_transport *t;
	struct sctp_endpoint *ep = asoc->ep;

	 
	switch (param.p->type) {
	case SCTP_PARAM_IPV6_ADDRESS:
		if (PF_INET6 != asoc->base.sk->sk_family)
			break;
		goto do_addr_param;

	case SCTP_PARAM_IPV4_ADDRESS:
		 
		if (ipv6_only_sock(asoc->base.sk))
			break;
do_addr_param:
		af = sctp_get_af_specific(param_type2af(param.p->type));
		af->from_addr_param(&addr, param.addr, htons(asoc->peer.port), 0);
		scope = sctp_scope(peer_addr);
		if (sctp_in_scope(net, &addr, scope))
			if (!sctp_assoc_add_peer(asoc, &addr, gfp, SCTP_UNCONFIRMED))
				return 0;
		break;

	case SCTP_PARAM_COOKIE_PRESERVATIVE:
		if (!net->sctp.cookie_preserve_enable)
			break;

		stale = ntohl(param.life->lifespan_increment);

		 
		asoc->cookie_life = ktime_add_ms(asoc->cookie_life, stale);
		break;

	case SCTP_PARAM_HOST_NAME_ADDRESS:
		pr_debug("%s: unimplemented SCTP_HOST_NAME_ADDRESS\n", __func__);
		break;

	case SCTP_PARAM_SUPPORTED_ADDRESS_TYPES:
		 
		asoc->peer.ipv4_address = 0;
		asoc->peer.ipv6_address = 0;

		 
		if (peer_addr->sa.sa_family == AF_INET6)
			asoc->peer.ipv6_address = 1;
		else if (peer_addr->sa.sa_family == AF_INET)
			asoc->peer.ipv4_address = 1;

		 
		sat = ntohs(param.p->length) - sizeof(sctp_paramhdr_t);
		if (sat)
			sat /= sizeof(__u16);

		for (i = 0; i < sat; ++i) {
			switch (param.sat->types[i]) {
			case SCTP_PARAM_IPV4_ADDRESS:
				asoc->peer.ipv4_address = 1;
				break;

			case SCTP_PARAM_IPV6_ADDRESS:
				if (PF_INET6 == asoc->base.sk->sk_family)
					asoc->peer.ipv6_address = 1;
				break;

			case SCTP_PARAM_HOST_NAME_ADDRESS:
				asoc->peer.hostname_address = 1;
				break;

			default:  
				break;
			}
		}
		break;

	case SCTP_PARAM_STATE_COOKIE:
		asoc->peer.cookie_len =
			ntohs(param.p->length) - sizeof(sctp_paramhdr_t);
		asoc->peer.cookie = param.cookie->body;
		break;

	case SCTP_PARAM_HEARTBEAT_INFO:
		 
		break;

	case SCTP_PARAM_UNRECOGNIZED_PARAMETERS:
		 
		break;

	case SCTP_PARAM_ECN_CAPABLE:
		asoc->peer.ecn_capable = 1;
		break;

	case SCTP_PARAM_ADAPTATION_LAYER_IND:
		asoc->peer.adaptation_ind = ntohl(param.aind->adaptation_ind);
		break;

	case SCTP_PARAM_SET_PRIMARY:
		if (!net->sctp.addip_enable)
			goto fall_through;

		addr_param = param.v + sizeof(sctp_addip_param_t);

		af = sctp_get_af_specific(param_type2af(param.p->type));
		af->from_addr_param(&addr, addr_param,
				    htons(asoc->peer.port), 0);

		 
		if (!af->addr_valid(&addr, NULL, NULL))
			break;

		t = sctp_assoc_lookup_paddr(asoc, &addr);
		if (!t)
			break;

		sctp_assoc_set_primary(asoc, t);
		break;

	case SCTP_PARAM_SUPPORTED_EXT:
		sctp_process_ext_param(asoc, param);
		break;

	case SCTP_PARAM_FWD_TSN_SUPPORT:
		if (net->sctp.prsctp_enable) {
			asoc->peer.prsctp_capable = 1;
			break;
		}
		 
		goto fall_through;

	case SCTP_PARAM_RANDOM:
		if (!ep->auth_enable)
			goto fall_through;

		 
		asoc->peer.peer_random = kmemdup(param.p,
					    ntohs(param.p->length), gfp);
		if (!asoc->peer.peer_random) {
			retval = 0;
			break;
		}
		break;

	case SCTP_PARAM_HMAC_ALGO:
		if (!ep->auth_enable)
			goto fall_through;

		 
		asoc->peer.peer_hmacs = kmemdup(param.p,
					    ntohs(param.p->length), gfp);
		if (!asoc->peer.peer_hmacs) {
			retval = 0;
			break;
		}

		 
		sctp_auth_asoc_set_default_hmac(asoc, param.hmac_algo);
		break;

	case SCTP_PARAM_CHUNKS:
		if (!ep->auth_enable)
			goto fall_through;

		asoc->peer.peer_chunks = kmemdup(param.p,
					    ntohs(param.p->length), gfp);
		if (!asoc->peer.peer_chunks)
			retval = 0;
		break;
fall_through:
	default:
		 
		pr_debug("%s: ignoring param:%d for association:%p.\n",
			 __func__, ntohs(param.p->type), asoc);
		break;
	}

	return retval;
}