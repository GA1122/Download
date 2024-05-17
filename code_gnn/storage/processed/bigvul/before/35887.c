int sctp_process_init(struct sctp_association *asoc, struct sctp_chunk *chunk,
		      const union sctp_addr *peer_addr,
		      sctp_init_chunk_t *peer_init, gfp_t gfp)
{
	struct net *net = sock_net(asoc->base.sk);
	union sctp_params param;
	struct sctp_transport *transport;
	struct list_head *pos, *temp;
	struct sctp_af *af;
	union sctp_addr addr;
	char *cookie;
	int src_match = 0;

	 

	 
	if (!sctp_assoc_add_peer(asoc, peer_addr, gfp, SCTP_ACTIVE))
		goto nomem;

	if (sctp_cmp_addr_exact(sctp_source(chunk), peer_addr))
		src_match = 1;

	 
	sctp_walk_params(param, peer_init, init_hdr.params) {
		if (!src_match && (param.p->type == SCTP_PARAM_IPV4_ADDRESS ||
		    param.p->type == SCTP_PARAM_IPV6_ADDRESS)) {
			af = sctp_get_af_specific(param_type2af(param.p->type));
			af->from_addr_param(&addr, param.addr,
					    chunk->sctp_hdr->source, 0);
			if (sctp_cmp_addr_exact(sctp_source(chunk), &addr))
				src_match = 1;
		}

		if (!sctp_process_param(asoc, param, peer_addr, gfp))
			goto clean_up;
	}

	 
	if (!src_match)
		goto clean_up;

	 
	if (asoc->peer.auth_capable && (!asoc->peer.peer_random ||
					!asoc->peer.peer_hmacs))
		asoc->peer.auth_capable = 0;

	 
	if (!net->sctp.addip_noauth &&
	     (asoc->peer.asconf_capable && !asoc->peer.auth_capable)) {
		asoc->peer.addip_disabled_mask |= (SCTP_PARAM_ADD_IP |
						  SCTP_PARAM_DEL_IP |
						  SCTP_PARAM_SET_PRIMARY);
		asoc->peer.asconf_capable = 0;
		goto clean_up;
	}

	 
	list_for_each_safe(pos, temp, &asoc->peer.transport_addr_list) {
		transport = list_entry(pos, struct sctp_transport, transports);
		if (transport->state == SCTP_UNKNOWN) {
			sctp_assoc_rm_peer(asoc, transport);
		}
	}

	 
	asoc->peer.i.init_tag =
		ntohl(peer_init->init_hdr.init_tag);
	asoc->peer.i.a_rwnd =
		ntohl(peer_init->init_hdr.a_rwnd);
	asoc->peer.i.num_outbound_streams =
		ntohs(peer_init->init_hdr.num_outbound_streams);
	asoc->peer.i.num_inbound_streams =
		ntohs(peer_init->init_hdr.num_inbound_streams);
	asoc->peer.i.initial_tsn =
		ntohl(peer_init->init_hdr.initial_tsn);

	 
	if (asoc->c.sinit_num_ostreams  >
	    ntohs(peer_init->init_hdr.num_inbound_streams)) {
		asoc->c.sinit_num_ostreams =
			ntohs(peer_init->init_hdr.num_inbound_streams);
	}

	if (asoc->c.sinit_max_instreams >
	    ntohs(peer_init->init_hdr.num_outbound_streams)) {
		asoc->c.sinit_max_instreams =
			ntohs(peer_init->init_hdr.num_outbound_streams);
	}

	 
	asoc->c.peer_vtag = asoc->peer.i.init_tag;

	 
	asoc->peer.rwnd = asoc->peer.i.a_rwnd;

	 
	cookie = asoc->peer.cookie;
	if (cookie) {
		asoc->peer.cookie = kmemdup(cookie, asoc->peer.cookie_len, gfp);
		if (!asoc->peer.cookie)
			goto clean_up;
	}

	 
	list_for_each_entry(transport, &asoc->peer.transport_addr_list,
			transports) {
		transport->ssthresh = asoc->peer.i.a_rwnd;
	}

	 
	if (!sctp_tsnmap_init(&asoc->peer.tsn_map, SCTP_TSN_MAP_INITIAL,
				asoc->peer.i.initial_tsn, gfp))
		goto clean_up;

	 

	 
	if (!asoc->temp) {
		int error;

		asoc->ssnmap = sctp_ssnmap_new(asoc->c.sinit_max_instreams,
					       asoc->c.sinit_num_ostreams, gfp);
		if (!asoc->ssnmap)
			goto clean_up;

		error = sctp_assoc_set_id(asoc, gfp);
		if (error)
			goto clean_up;
	}

	 
	asoc->peer.addip_serial = asoc->peer.i.initial_tsn - 1;
	return 1;

clean_up:
	 
	list_for_each_safe(pos, temp, &asoc->peer.transport_addr_list) {
		transport = list_entry(pos, struct sctp_transport, transports);
		if (transport->state != SCTP_ACTIVE)
			sctp_assoc_rm_peer(asoc, transport);
	}

nomem:
	return 0;
}
