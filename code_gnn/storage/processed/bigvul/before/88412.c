daemon_msg_startcap_req(uint8 ver, struct daemon_slpars *pars, uint32 plen,
    char *source, struct session **sessionp,
    struct rpcap_sampling *samp_param _U_)
{
	char errbuf[PCAP_ERRBUF_SIZE];		 
	char errmsgbuf[PCAP_ERRBUF_SIZE];	 
	char portdata[PCAP_BUF_SIZE];		 
	char peerhost[PCAP_BUF_SIZE];		 
	struct session *session = NULL;		 
	int status;
	char sendbuf[RPCAP_NETBUF_SIZE];	 
	int sendbufidx = 0;			 

	struct addrinfo hints;			 
	struct addrinfo *addrinfo;		 
	struct sockaddr_storage saddr;		 
	socklen_t saddrlen;			 
	int ret;				 

	struct rpcap_startcapreq startcapreq;		 
	struct rpcap_startcapreply *startcapreply;	 
	int serveropen_dp;							 

	addrinfo = NULL;

	status = rpcapd_recv(pars->sockctrl, (char *) &startcapreq,
	    sizeof(struct rpcap_startcapreq), &plen, errmsgbuf);
	if (status == -1)
	{
		goto fatal_error;
	}
	if (status == -2)
	{
		goto error;
	}

	startcapreq.flags = ntohs(startcapreq.flags);

	session = malloc(sizeof(struct session));
	if (session == NULL)
	{
		pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Can't allocate session structure");
		goto error;
	}

	session->sockdata = INVALID_SOCKET;
	session->have_thread = 0;
#ifdef _WIN32
	session->thread = INVALID_HANDLE_VALUE;
#else
	memset(&session->thread, 0, sizeof(session->thread));
#endif

	if ((session->fp = pcap_open_live(source,
			ntohl(startcapreq.snaplen),
			(startcapreq.flags & RPCAP_STARTCAPREQ_FLAG_PROMISC) ? 1 : 0  ,
			ntohl(startcapreq.read_timeout),
			errmsgbuf)) == NULL)
		goto error;

#if 0
	fp->rmt_samp.method = samp_param->method;
	fp->rmt_samp.value = samp_param->value;
#endif

	 
	serveropen_dp = (startcapreq.flags & RPCAP_STARTCAPREQ_FLAG_SERVEROPEN) || (startcapreq.flags & RPCAP_STARTCAPREQ_FLAG_DGRAM) || pars->isactive;

	 
	saddrlen = sizeof(struct sockaddr_storage);
	if (getpeername(pars->sockctrl, (struct sockaddr *) &saddr, &saddrlen) == -1)
	{
		sock_geterror("getpeername()", errmsgbuf, PCAP_ERRBUF_SIZE);
		goto error;
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = (startcapreq.flags & RPCAP_STARTCAPREQ_FLAG_DGRAM) ? SOCK_DGRAM : SOCK_STREAM;
	hints.ai_family = saddr.ss_family;

	if (serveropen_dp)		 
	{
		pcap_snprintf(portdata, sizeof portdata, "%d", ntohs(startcapreq.portdata));

		if (getnameinfo((struct sockaddr *) &saddr, saddrlen, peerhost,
				sizeof(peerhost), NULL, 0, NI_NUMERICHOST))
		{
			sock_geterror("getnameinfo()", errmsgbuf, PCAP_ERRBUF_SIZE);
			goto error;
		}

		if (sock_initaddress(peerhost, portdata, &hints, &addrinfo, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
			goto error;

		if ((session->sockdata = sock_open(addrinfo, SOCKOPEN_CLIENT, 0, errmsgbuf, PCAP_ERRBUF_SIZE)) == INVALID_SOCKET)
			goto error;
	}
	else		 
	{
		hints.ai_flags = AI_PASSIVE;

		if (sock_initaddress(NULL, "0", &hints, &addrinfo, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
			goto error;

		if ((session->sockdata = sock_open(addrinfo, SOCKOPEN_SERVER, 1  , errmsgbuf, PCAP_ERRBUF_SIZE)) == INVALID_SOCKET)
			goto error;

		saddrlen = sizeof(struct sockaddr_storage);
		if (getsockname(session->sockdata, (struct sockaddr *) &saddr, &saddrlen) == -1)
		{
			sock_geterror("getsockname()", errmsgbuf, PCAP_ERRBUF_SIZE);
			goto error;
		}

		if (getnameinfo((struct sockaddr *) &saddr, saddrlen, NULL,
				0, portdata, sizeof(portdata), NI_NUMERICSERV))
		{
			sock_geterror("getnameinfo()", errmsgbuf, PCAP_ERRBUF_SIZE);
			goto error;
		}
	}

	freeaddrinfo(addrinfo);
	addrinfo = NULL;

	session->sockctrl = pars->sockctrl;
	session->protocol_version = ver;

	ret = daemon_unpackapplyfilter(pars->sockctrl, session, &plen, errmsgbuf);
	if (ret == -1)
	{
		goto fatal_error;
	}
	if (ret == -2)
	{
		goto error;
	}

	if (sock_bufferize(NULL, sizeof(struct rpcap_header), NULL, &sendbufidx,
	    RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	rpcap_createhdr((struct rpcap_header *) sendbuf, ver,
	    RPCAP_MSG_STARTCAP_REPLY, 0, sizeof(struct rpcap_startcapreply));

	startcapreply = (struct rpcap_startcapreply *) &sendbuf[sendbufidx];

	if (sock_bufferize(NULL, sizeof(struct rpcap_startcapreply), NULL,
	    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	memset(startcapreply, 0, sizeof(struct rpcap_startcapreply));
	startcapreply->bufsize = htonl(pcap_bufsize(session->fp));

	if (!serveropen_dp)
	{
		unsigned short port = (unsigned short)strtoul(portdata,NULL,10);
		startcapreply->portdata = htons(port);
	}

	if (sock_send(pars->sockctrl, sendbuf, sendbufidx, errbuf, PCAP_ERRBUF_SIZE) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		goto fatal_error;
	}

	if (!serveropen_dp)
	{
		SOCKET socktemp;	 

		saddrlen = sizeof(struct sockaddr_storage);

		socktemp = accept(session->sockdata, (struct sockaddr *) &saddr, &saddrlen);

		if (socktemp == INVALID_SOCKET)
		{
			sock_geterror("accept()", errbuf, PCAP_ERRBUF_SIZE);
			rpcapd_log(LOGPRIO_ERROR, "Accept of data connection failed: %s",
			    errbuf);
			goto error;
		}

		sock_close(session->sockdata, NULL, 0);
		session->sockdata = socktemp;
	}

#ifdef _WIN32
	session->thread = (HANDLE)_beginthreadex(NULL, 0, daemon_thrdatamain,
	    (void *) session, 0, NULL);
	if (session->thread == 0)
	{
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "Error creating the data thread");
		goto error;
	}
#else
	ret = pthread_create(&session->thread, NULL, daemon_thrdatamain,
	    (void *) session);
	if (ret != 0)
	{
		pcap_fmt_errmsg_for_errno(errbuf, PCAP_ERRBUF_SIZE,
		    ret, "Error creating the data thread");
		goto error;
	}
#endif
	session->have_thread = 1;

	if (rpcapd_discard(pars->sockctrl, plen) == -1)
		goto fatal_error;

	*sessionp = session;
	return 0;

error:
	*sessionp = NULL;

	if (addrinfo)
		freeaddrinfo(addrinfo);

	if (session)
	{
		session_close(session);
		free(session);
	}

	if (rpcap_senderror(pars->sockctrl, ver, PCAP_ERR_STARTCAPTURE,
	    errmsgbuf, errbuf) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}

	if (rpcapd_discard(pars->sockctrl, plen) == -1)
	{
		return -1;
	}

	return 0;

fatal_error:
	*sessionp = NULL;

	if (session)
	{
		session_close(session);
		free(session);
	}

	return -1;
}