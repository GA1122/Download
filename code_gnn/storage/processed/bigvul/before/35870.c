struct sctp_chunk *sctp_make_init_ack(const struct sctp_association *asoc,
				 const struct sctp_chunk *chunk,
				 gfp_t gfp, int unkparam_len)
{
	sctp_inithdr_t initack;
	struct sctp_chunk *retval;
	union sctp_params addrs;
	struct sctp_sock *sp;
	int addrs_len;
	sctp_cookie_param_t *cookie;
	int cookie_len;
	size_t chunksize;
	sctp_adaptation_ind_param_t aiparam;
	sctp_supported_ext_param_t ext_param;
	int num_ext = 0;
	__u8 extensions[3];
	sctp_paramhdr_t *auth_chunks = NULL,
			*auth_hmacs = NULL,
			*auth_random = NULL;

	retval = NULL;

	 
	addrs = sctp_bind_addrs_to_raw(&asoc->base.bind_addr, &addrs_len, gfp);

	initack.init_tag	        = htonl(asoc->c.my_vtag);
	initack.a_rwnd			= htonl(asoc->rwnd);
	initack.num_outbound_streams	= htons(asoc->c.sinit_num_ostreams);
	initack.num_inbound_streams	= htons(asoc->c.sinit_max_instreams);
	initack.initial_tsn		= htonl(asoc->c.initial_tsn);

	 
	cookie = sctp_pack_cookie(asoc->ep, asoc, chunk, &cookie_len,
				  addrs.v, addrs_len);
	if (!cookie)
		goto nomem_cookie;

	 
	sp = sctp_sk(asoc->base.sk);
	chunksize = sizeof(initack) + addrs_len + cookie_len + unkparam_len;

	 
	if (asoc->peer.ecn_capable)
		chunksize += sizeof(ecap_param);

	if (asoc->peer.prsctp_capable)
		chunksize += sizeof(prsctp_param);

	if (asoc->peer.asconf_capable) {
		extensions[num_ext] = SCTP_CID_ASCONF;
		extensions[num_ext+1] = SCTP_CID_ASCONF_ACK;
		num_ext += 2;
	}

	if (sp->adaptation_ind)
		chunksize += sizeof(aiparam);

	if (asoc->peer.auth_capable) {
		auth_random = (sctp_paramhdr_t *)asoc->c.auth_random;
		chunksize += ntohs(auth_random->length);

		auth_hmacs = (sctp_paramhdr_t *)asoc->c.auth_hmacs;
		if (auth_hmacs->length)
			chunksize += WORD_ROUND(ntohs(auth_hmacs->length));
		else
			auth_hmacs = NULL;

		auth_chunks = (sctp_paramhdr_t *)asoc->c.auth_chunks;
		if (auth_chunks->length)
			chunksize += WORD_ROUND(ntohs(auth_chunks->length));
		else
			auth_chunks = NULL;

		extensions[num_ext] = SCTP_CID_AUTH;
		num_ext += 1;
	}

	if (num_ext)
		chunksize += WORD_ROUND(sizeof(sctp_supported_ext_param_t) +
					num_ext);

	 
	retval = sctp_make_control(asoc, SCTP_CID_INIT_ACK, 0, chunksize);
	if (!retval)
		goto nomem_chunk;

	 
	retval->transport = chunk->transport;

	retval->subh.init_hdr =
		sctp_addto_chunk(retval, sizeof(initack), &initack);
	retval->param_hdr.v = sctp_addto_chunk(retval, addrs_len, addrs.v);
	sctp_addto_chunk(retval, cookie_len, cookie);
	if (asoc->peer.ecn_capable)
		sctp_addto_chunk(retval, sizeof(ecap_param), &ecap_param);
	if (num_ext) {
		ext_param.param_hdr.type = SCTP_PARAM_SUPPORTED_EXT;
		ext_param.param_hdr.length =
			    htons(sizeof(sctp_supported_ext_param_t) + num_ext);
		sctp_addto_chunk(retval, sizeof(sctp_supported_ext_param_t),
				 &ext_param);
		sctp_addto_param(retval, num_ext, extensions);
	}
	if (asoc->peer.prsctp_capable)
		sctp_addto_chunk(retval, sizeof(prsctp_param), &prsctp_param);

	if (sp->adaptation_ind) {
		aiparam.param_hdr.type = SCTP_PARAM_ADAPTATION_LAYER_IND;
		aiparam.param_hdr.length = htons(sizeof(aiparam));
		aiparam.adaptation_ind = htonl(sp->adaptation_ind);
		sctp_addto_chunk(retval, sizeof(aiparam), &aiparam);
	}

	if (asoc->peer.auth_capable) {
		sctp_addto_chunk(retval, ntohs(auth_random->length),
				 auth_random);
		if (auth_hmacs)
			sctp_addto_chunk(retval, ntohs(auth_hmacs->length),
					auth_hmacs);
		if (auth_chunks)
			sctp_addto_chunk(retval, ntohs(auth_chunks->length),
					auth_chunks);
	}

	 
	retval->asoc = (struct sctp_association *) asoc;

nomem_chunk:
	kfree(cookie);
nomem_cookie:
	kfree(addrs.v);
	return retval;
}