send_query_to_peer(peer_t *p)
{
	 
#define PROBE_LOCAL_ADDR  

	if (p->p_fd == -1) {
		int fd, family;
		len_and_sockaddr *local_lsa;

		family = p->p_lsa->u.sa.sa_family;
		p->p_fd = fd = xsocket_type(&local_lsa, family, SOCK_DGRAM);
		 
		PROBE_LOCAL_ADDR
		xbind(fd, &local_lsa->u.sa, local_lsa->len);
		PROBE_LOCAL_ADDR
#if ENABLE_FEATURE_IPV6
		if (family == AF_INET)
#endif
			setsockopt_int(fd, IPPROTO_IP, IP_TOS, IPTOS_LOWDELAY);
		free(local_lsa);
	}

	 
	VERB1 bb_error_msg("sending query to %s", p->p_dotted);

	 
	p->p_xmt_msg.m_xmttime.int_partl = rand();
	p->p_xmt_msg.m_xmttime.fractionl = rand();
	p->p_xmttime = gettime1900d();

	 
	p->reachable_bits <<= 1;

	if (do_sendto(p->p_fd,   NULL,   &p->p_lsa->u.sa,   p->p_lsa->len,
			&p->p_xmt_msg, NTP_MSGSIZE_NOAUTH) == -1
	) {
		close(p->p_fd);
		p->p_fd = -1;
		 
		set_next(p, RETRY_INTERVAL);
		return;
	}

	set_next(p, RESPONSE_INTERVAL);
}
