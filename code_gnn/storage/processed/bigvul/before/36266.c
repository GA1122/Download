static struct sctp_association *sctp_association_init(struct sctp_association *asoc,
					  const struct sctp_endpoint *ep,
					  const struct sock *sk,
					  sctp_scope_t scope,
					  gfp_t gfp)
{
	struct net *net = sock_net(sk);
	struct sctp_sock *sp;
	int i;
	sctp_paramhdr_t *p;
	int err;

	 
	sp = sctp_sk((struct sock *)sk);

	 
	asoc->ep = (struct sctp_endpoint *)ep;
	asoc->base.sk = (struct sock *)sk;

	sctp_endpoint_hold(asoc->ep);
	sock_hold(asoc->base.sk);

	 
	asoc->base.type = SCTP_EP_TYPE_ASSOCIATION;

	 
	atomic_set(&asoc->base.refcnt, 1);

	 
	sctp_bind_addr_init(&asoc->base.bind_addr, ep->base.bind_addr.port);

	asoc->state = SCTP_STATE_CLOSED;
	asoc->cookie_life = ms_to_ktime(sp->assocparams.sasoc_cookie_life);
	asoc->user_frag = sp->user_frag;

	 
	asoc->max_retrans = sp->assocparams.sasoc_asocmaxrxt;
	asoc->pf_retrans  = net->sctp.pf_retrans;

	asoc->rto_initial = msecs_to_jiffies(sp->rtoinfo.srto_initial);
	asoc->rto_max = msecs_to_jiffies(sp->rtoinfo.srto_max);
	asoc->rto_min = msecs_to_jiffies(sp->rtoinfo.srto_min);

	 
	asoc->hbinterval = msecs_to_jiffies(sp->hbinterval);

	 
	asoc->pathmaxrxt = sp->pathmaxrxt;

	 
	asoc->pathmtu = sp->pathmtu;

	 
	asoc->sackdelay = msecs_to_jiffies(sp->sackdelay);
	asoc->sackfreq = sp->sackfreq;

	 
	asoc->param_flags = sp->param_flags;

	 
	asoc->max_burst = sp->max_burst;

	 
	asoc->timeouts[SCTP_EVENT_TIMEOUT_T1_COOKIE] = asoc->rto_initial;
	asoc->timeouts[SCTP_EVENT_TIMEOUT_T1_INIT] = asoc->rto_initial;
	asoc->timeouts[SCTP_EVENT_TIMEOUT_T2_SHUTDOWN] = asoc->rto_initial;

	 
	asoc->timeouts[SCTP_EVENT_TIMEOUT_T5_SHUTDOWN_GUARD]
		= 5 * asoc->rto_max;

	asoc->timeouts[SCTP_EVENT_TIMEOUT_SACK] = asoc->sackdelay;
	asoc->timeouts[SCTP_EVENT_TIMEOUT_AUTOCLOSE] = sp->autoclose * HZ;

	 
	for (i = SCTP_EVENT_TIMEOUT_NONE; i < SCTP_NUM_TIMEOUT_TYPES; ++i)
		setup_timer(&asoc->timers[i], sctp_timer_events[i],
				(unsigned long)asoc);

	 
	asoc->c.sinit_max_instreams = sp->initmsg.sinit_max_instreams;
	asoc->c.sinit_num_ostreams  = sp->initmsg.sinit_num_ostreams;
	asoc->max_init_attempts	= sp->initmsg.sinit_max_attempts;

	asoc->max_init_timeo =
		 msecs_to_jiffies(sp->initmsg.sinit_max_init_timeo);

	 
	if ((sk->sk_rcvbuf/2) < SCTP_DEFAULT_MINWINDOW)
		asoc->rwnd = SCTP_DEFAULT_MINWINDOW;
	else
		asoc->rwnd = sk->sk_rcvbuf/2;

	asoc->a_rwnd = asoc->rwnd;

	 
	asoc->peer.rwnd = SCTP_DEFAULT_MAXWINDOW;

	 
	atomic_set(&asoc->rmem_alloc, 0);

	init_waitqueue_head(&asoc->wait);

	asoc->c.my_vtag = sctp_generate_tag(ep);
	asoc->c.my_port = ep->base.bind_addr.port;

	asoc->c.initial_tsn = sctp_generate_tsn(ep);

	asoc->next_tsn = asoc->c.initial_tsn;

	asoc->ctsn_ack_point = asoc->next_tsn - 1;
	asoc->adv_peer_ack_point = asoc->ctsn_ack_point;
	asoc->highest_sacked = asoc->ctsn_ack_point;
	asoc->last_cwr_tsn = asoc->ctsn_ack_point;

	 
	asoc->addip_serial = asoc->c.initial_tsn;

	INIT_LIST_HEAD(&asoc->addip_chunk_list);
	INIT_LIST_HEAD(&asoc->asconf_ack_list);

	 
	INIT_LIST_HEAD(&asoc->peer.transport_addr_list);

	 
	asoc->peer.sack_needed = 1;
	asoc->peer.sack_generation = 1;

	 
	if (net->sctp.addip_noauth)
		asoc->peer.asconf_capable = 1;

	 
	sctp_inq_init(&asoc->base.inqueue);
	sctp_inq_set_th_handler(&asoc->base.inqueue, sctp_assoc_bh_rcv);

	 
	sctp_outq_init(asoc, &asoc->outqueue);

	if (!sctp_ulpq_init(&asoc->ulpq, asoc))
		goto fail_init;

	 
	asoc->peer.ipv4_address = 1;
	if (asoc->base.sk->sk_family == PF_INET6)
		asoc->peer.ipv6_address = 1;
	INIT_LIST_HEAD(&asoc->asocs);

	asoc->default_stream = sp->default_stream;
	asoc->default_ppid = sp->default_ppid;
	asoc->default_flags = sp->default_flags;
	asoc->default_context = sp->default_context;
	asoc->default_timetolive = sp->default_timetolive;
	asoc->default_rcv_context = sp->default_rcv_context;

	 
	INIT_LIST_HEAD(&asoc->endpoint_shared_keys);
	err = sctp_auth_asoc_copy_shkeys(ep, asoc, gfp);
	if (err)
		goto fail_init;

	asoc->active_key_id = ep->active_key_id;

	 
	if (ep->auth_hmacs_list)
		memcpy(asoc->c.auth_hmacs, ep->auth_hmacs_list,
			ntohs(ep->auth_hmacs_list->param_hdr.length));
	if (ep->auth_chunk_list)
		memcpy(asoc->c.auth_chunks, ep->auth_chunk_list,
			ntohs(ep->auth_chunk_list->param_hdr.length));

	 
	p = (sctp_paramhdr_t *)asoc->c.auth_random;
	p->type = SCTP_PARAM_RANDOM;
	p->length = htons(sizeof(sctp_paramhdr_t) + SCTP_AUTH_RANDOM_LENGTH);
	get_random_bytes(p+1, SCTP_AUTH_RANDOM_LENGTH);

	return asoc;

fail_init:
	sock_put(asoc->base.sk);
	sctp_endpoint_put(asoc->ep);
	return NULL;
}
