static __be16 sctp_process_asconf_param(struct sctp_association *asoc,
				       struct sctp_chunk *asconf,
				       sctp_addip_param_t *asconf_param)
{
	struct sctp_transport *peer;
	struct sctp_af *af;
	union sctp_addr	addr;
	union sctp_addr_param *addr_param;

	addr_param = (void *)asconf_param + sizeof(sctp_addip_param_t);

	if (asconf_param->param_hdr.type != SCTP_PARAM_ADD_IP &&
	    asconf_param->param_hdr.type != SCTP_PARAM_DEL_IP &&
	    asconf_param->param_hdr.type != SCTP_PARAM_SET_PRIMARY)
		return SCTP_ERROR_UNKNOWN_PARAM;

	switch (addr_param->p.type) {
	case SCTP_PARAM_IPV6_ADDRESS:
		if (!asoc->peer.ipv6_address)
			return SCTP_ERROR_DNS_FAILED;
		break;
	case SCTP_PARAM_IPV4_ADDRESS:
		if (!asoc->peer.ipv4_address)
			return SCTP_ERROR_DNS_FAILED;
		break;
	default:
		return SCTP_ERROR_DNS_FAILED;
	}

	af = sctp_get_af_specific(param_type2af(addr_param->p.type));
	if (unlikely(!af))
		return SCTP_ERROR_DNS_FAILED;

	af->from_addr_param(&addr, addr_param, htons(asoc->peer.port), 0);

	 
	if (!af->is_any(&addr) && !af->addr_valid(&addr, NULL, asconf->skb))
		return SCTP_ERROR_DNS_FAILED;

	switch (asconf_param->param_hdr.type) {
	case SCTP_PARAM_ADD_IP:
		 
		if (af->is_any(&addr))
			memcpy(&addr, &asconf->source, sizeof(addr));

		 

		peer = sctp_assoc_add_peer(asoc, &addr, GFP_ATOMIC, SCTP_UNCONFIRMED);
		if (!peer)
			return SCTP_ERROR_RSRC_LOW;

		 
		if (!mod_timer(&peer->hb_timer, sctp_transport_timeout(peer)))
			sctp_transport_hold(peer);
		asoc->new_transport = peer;
		break;
	case SCTP_PARAM_DEL_IP:
		 
		if (asoc->peer.transport_count == 1)
			return SCTP_ERROR_DEL_LAST_IP;

		 
		if (sctp_cmp_addr_exact(&asconf->source, &addr))
			return SCTP_ERROR_DEL_SRC_IP;

		 
		if (af->is_any(&addr)) {
			sctp_assoc_set_primary(asoc, asconf->transport);
			sctp_assoc_del_nonprimary_peers(asoc,
							asconf->transport);
		} else
			sctp_assoc_del_peer(asoc, &addr);
		break;
	case SCTP_PARAM_SET_PRIMARY:
		 
		if (af->is_any(&addr))
			memcpy(&addr.v4, sctp_source(asconf), sizeof(addr));

		peer = sctp_assoc_lookup_paddr(asoc, &addr);
		if (!peer)
			return SCTP_ERROR_DNS_FAILED;

		sctp_assoc_set_primary(asoc, peer);
		break;
	}

 	return SCTP_ERROR_NO_ERROR;
 }
