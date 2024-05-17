SCTP_STATIC int sctp_init_sock(struct sock *sk)
{
	struct sctp_endpoint *ep;
	struct sctp_sock *sp;

	SCTP_DEBUG_PRINTK("sctp_init_sock(sk: %p)\n", sk);

	sp = sctp_sk(sk);

	 
	switch (sk->sk_type) {
	case SOCK_SEQPACKET:
		sp->type = SCTP_SOCKET_UDP;
		break;
	case SOCK_STREAM:
		sp->type = SCTP_SOCKET_TCP;
		break;
	default:
		return -ESOCKTNOSUPPORT;
	}

	 
	sp->default_stream = 0;
	sp->default_ppid = 0;
	sp->default_flags = 0;
	sp->default_context = 0;
	sp->default_timetolive = 0;

	sp->default_rcv_context = 0;

	 
	sp->initmsg.sinit_num_ostreams   = sctp_max_outstreams;
	sp->initmsg.sinit_max_instreams  = sctp_max_instreams;
	sp->initmsg.sinit_max_attempts   = sctp_max_retrans_init;
	sp->initmsg.sinit_max_init_timeo = sctp_rto_max;

	 
	sp->rtoinfo.srto_initial = sctp_rto_initial;
	sp->rtoinfo.srto_max     = sctp_rto_max;
	sp->rtoinfo.srto_min     = sctp_rto_min;

	 
	sp->assocparams.sasoc_asocmaxrxt = sctp_max_retrans_association;
	sp->assocparams.sasoc_number_peer_destinations = 0;
	sp->assocparams.sasoc_peer_rwnd = 0;
	sp->assocparams.sasoc_local_rwnd = 0;
	sp->assocparams.sasoc_cookie_life = sctp_valid_cookie_life;

	 
	memset(&sp->subscribe, 0, sizeof(struct sctp_event_subscribe));

	 
	sp->hbinterval  = sctp_hb_interval;
	sp->pathmaxrxt  = sctp_max_retrans_path;
	sp->pathmtu     = 0;  
	sp->sackdelay   = sctp_sack_timeout;
	sp->param_flags = SPP_HB_ENABLE |
			  SPP_PMTUD_ENABLE |
			  SPP_SACKDELAY_ENABLE;

	 
	sp->disable_fragments = 0;

	 
	sp->nodelay           = 0;

	 
	sp->v4mapped          = 1;

	 
	sp->autoclose         = 0;

	 
	sp->user_frag         = 0;

	sp->adaptation_ind = 0;

	sp->pf = sctp_get_pf_specific(sk->sk_family);

	 
	sp->pd_mode           = 0;
	skb_queue_head_init(&sp->pd_lobby);

	 
	ep = sctp_endpoint_new(sk, GFP_KERNEL);
	if (!ep)
		return -ENOMEM;

	sp->ep = ep;
	sp->hmac = NULL;

	SCTP_DBG_OBJCNT_INC(sock);
	return 0;
}
