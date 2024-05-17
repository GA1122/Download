struct sctp_chunk *sctp_make_init(const struct sctp_association *asoc,
			     const struct sctp_bind_addr *bp,
			     gfp_t gfp, int vparam_len)
{
	struct net *net = sock_net(asoc->base.sk);
	struct sctp_endpoint *ep = asoc->ep;
	sctp_inithdr_t init;
	union sctp_params addrs;
	size_t chunksize;
	struct sctp_chunk *retval = NULL;
	int num_types, addrs_len = 0;
	struct sctp_sock *sp;
	sctp_supported_addrs_param_t sat;
	__be16 types[2];
	sctp_adaptation_ind_param_t aiparam;
	sctp_supported_ext_param_t ext_param;
	int num_ext = 0;
	__u8 extensions[3];
	sctp_paramhdr_t *auth_chunks = NULL,
			*auth_hmacs = NULL;

	 
	retval = NULL;

	 
	addrs = sctp_bind_addrs_to_raw(bp, &addrs_len, gfp);

	init.init_tag		   = htonl(asoc->c.my_vtag);
	init.a_rwnd		   = htonl(asoc->rwnd);
	init.num_outbound_streams  = htons(asoc->c.sinit_num_ostreams);
	init.num_inbound_streams   = htons(asoc->c.sinit_max_instreams);
	init.initial_tsn	   = htonl(asoc->c.initial_tsn);

	 
	sp = sctp_sk(asoc->base.sk);
	num_types = sp->pf->supported_addrs(sp, types);

	chunksize = sizeof(init) + addrs_len;
	chunksize += WORD_ROUND(SCTP_SAT_LEN(num_types));
	chunksize += sizeof(ecap_param);

	if (net->sctp.prsctp_enable)
		chunksize += sizeof(prsctp_param);

	 
	if (net->sctp.addip_enable) {
		extensions[num_ext] = SCTP_CID_ASCONF;
		extensions[num_ext+1] = SCTP_CID_ASCONF_ACK;
		num_ext += 2;
	}

	if (sp->adaptation_ind)
		chunksize += sizeof(aiparam);

	chunksize += vparam_len;

	 
	if (ep->auth_enable) {
		 
		chunksize += sizeof(asoc->c.auth_random);

		 
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

	 

	retval = sctp_make_control(asoc, SCTP_CID_INIT, 0, chunksize);
	if (!retval)
		goto nodata;

	retval->subh.init_hdr =
		sctp_addto_chunk(retval, sizeof(init), &init);
	retval->param_hdr.v =
		sctp_addto_chunk(retval, addrs_len, addrs.v);

	 
	sat.param_hdr.type = SCTP_PARAM_SUPPORTED_ADDRESS_TYPES;
	sat.param_hdr.length = htons(SCTP_SAT_LEN(num_types));
	sctp_addto_chunk(retval, sizeof(sat), &sat);
	sctp_addto_chunk(retval, num_types * sizeof(__u16), &types);

	sctp_addto_chunk(retval, sizeof(ecap_param), &ecap_param);

	 
	if (num_ext) {
		ext_param.param_hdr.type = SCTP_PARAM_SUPPORTED_EXT;
		ext_param.param_hdr.length =
			    htons(sizeof(sctp_supported_ext_param_t) + num_ext);
		sctp_addto_chunk(retval, sizeof(sctp_supported_ext_param_t),
				&ext_param);
		sctp_addto_param(retval, num_ext, extensions);
	}

	if (net->sctp.prsctp_enable)
		sctp_addto_chunk(retval, sizeof(prsctp_param), &prsctp_param);

	if (sp->adaptation_ind) {
		aiparam.param_hdr.type = SCTP_PARAM_ADAPTATION_LAYER_IND;
		aiparam.param_hdr.length = htons(sizeof(aiparam));
		aiparam.adaptation_ind = htonl(sp->adaptation_ind);
		sctp_addto_chunk(retval, sizeof(aiparam), &aiparam);
	}

	 
	if (ep->auth_enable) {
		sctp_addto_chunk(retval, sizeof(asoc->c.auth_random),
				 asoc->c.auth_random);
		if (auth_hmacs)
			sctp_addto_chunk(retval, ntohs(auth_hmacs->length),
					auth_hmacs);
		if (auth_chunks)
			sctp_addto_chunk(retval, ntohs(auth_chunks->length),
					auth_chunks);
	}
nodata:
	kfree(addrs.v);
	return retval;
}
