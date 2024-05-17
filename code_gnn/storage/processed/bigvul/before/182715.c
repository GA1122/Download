 daemon_msg_findallif_req(uint8 ver, struct daemon_slpars *pars, uint32 plen)
 {
 	char errbuf[PCAP_ERRBUF_SIZE];		 
 	char errmsgbuf[PCAP_ERRBUF_SIZE];	 
 	char sendbuf[RPCAP_NETBUF_SIZE];	 
 	int sendbufidx = 0;			 
 	pcap_if_t *alldevs = NULL;		 
  	pcap_if_t *d;				 
  	struct pcap_addr *address;		 
  	struct rpcap_findalldevs_if *findalldevs_if; 
// 	uint32 replylen;			 
  	uint16 nif = 0;				 
  
 	if (rpcapd_discard(pars->sockctrl, plen) == -1)
 	{
 		return -1;
 	}
 
 	if (pcap_findalldevs(&alldevs, errmsgbuf) == -1)
 		goto error;
 
 	if (alldevs == NULL)
 	{
 		if (rpcap_senderror(pars->sockctrl, ver, PCAP_ERR_NOREMOTEIF,
 			"No interfaces found! Make sure libpcap/WinPcap is properly installed"
 			" and you have the right to access to the remote device.",
 			errbuf) == -1)
 		{
 			rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
 			return -1;
 		}
  		return 0;
  	}
  
// 	 
// 	 
// 	replylen = 0;
  	for (d = alldevs; d != NULL; d = d->next)
  	{
  		nif++;
  
  		if (d->description)
			plen+= strlen(d->description);
// 			replylen += strlen(d->description);
  		if (d->name)
			plen+= strlen(d->name);
// 			replylen += strlen(d->name);
  
		plen+= sizeof(struct rpcap_findalldevs_if);
// 		replylen += sizeof(struct rpcap_findalldevs_if);
  
  		for (address = d->addresses; address != NULL; address = address->next)
  		{
 			 
 			switch (address->addr->sa_family)
 			{
 			case AF_INET:
  #ifdef AF_INET6
  			case AF_INET6:
  #endif
				plen+= (sizeof(struct rpcap_sockaddr) * 4);
// 				replylen += (sizeof(struct rpcap_sockaddr) * 4);
  				break;
  
  			default:
 				break;
 			}
 		}
 	}
 
 	if (sock_bufferize(NULL, sizeof(struct rpcap_header), NULL,
 	    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf,
 	    PCAP_ERRBUF_SIZE) == -1)
  		goto error;
  
  	rpcap_createhdr((struct rpcap_header *) sendbuf, ver,
	    RPCAP_MSG_FINDALLIF_REPLY, nif, plen);
// 	    RPCAP_MSG_FINDALLIF_REPLY, nif, replylen);
  
  	for (d = alldevs; d != NULL; d = d->next)
 	{
 		uint16 lname, ldescr;
 
 		findalldevs_if = (struct rpcap_findalldevs_if *) &sendbuf[sendbufidx];
 
 		if (sock_bufferize(NULL, sizeof(struct rpcap_findalldevs_if), NULL,
 		    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
 			goto error;
 
 		memset(findalldevs_if, 0, sizeof(struct rpcap_findalldevs_if));
 
 		if (d->description) ldescr = (short) strlen(d->description);
 		else ldescr = 0;
 		if (d->name) lname = (short) strlen(d->name);
 		else lname = 0;
 
 		findalldevs_if->desclen = htons(ldescr);
 		findalldevs_if->namelen = htons(lname);
 		findalldevs_if->flags = htonl(d->flags);
 
 		for (address = d->addresses; address != NULL; address = address->next)
 		{
 			 
 			switch (address->addr->sa_family)
 			{
 			case AF_INET:
 #ifdef AF_INET6
 			case AF_INET6:
 #endif
 				findalldevs_if->naddr++;
 				break;
 
 			default:
 				break;
 			}
 		}
 		findalldevs_if->naddr = htons(findalldevs_if->naddr);
 
 		if (sock_bufferize(d->name, lname, sendbuf, &sendbufidx,
 		    RPCAP_NETBUF_SIZE, SOCKBUF_BUFFERIZE, errmsgbuf,
 		    PCAP_ERRBUF_SIZE) == -1)
 			goto error;
 
 		if (sock_bufferize(d->description, ldescr, sendbuf, &sendbufidx,
 		    RPCAP_NETBUF_SIZE, SOCKBUF_BUFFERIZE, errmsgbuf,
 		    PCAP_ERRBUF_SIZE) == -1)
 			goto error;
 
 		for (address = d->addresses; address != NULL; address = address->next)
 		{
 			struct rpcap_sockaddr *sockaddr;
 
 			 
 			switch (address->addr->sa_family)
 			{
 			case AF_INET:
 #ifdef AF_INET6
 			case AF_INET6:
 #endif
 				sockaddr = (struct rpcap_sockaddr *) &sendbuf[sendbufidx];
 				if (sock_bufferize(NULL, sizeof(struct rpcap_sockaddr), NULL,
 				    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
 					goto error;
 				daemon_seraddr((struct sockaddr_storage *) address->addr, sockaddr);
 
 				sockaddr = (struct rpcap_sockaddr *) &sendbuf[sendbufidx];
 				if (sock_bufferize(NULL, sizeof(struct rpcap_sockaddr), NULL,
 				    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
 					goto error;
 				daemon_seraddr((struct sockaddr_storage *) address->netmask, sockaddr);
 
 				sockaddr = (struct rpcap_sockaddr *) &sendbuf[sendbufidx];
 				if (sock_bufferize(NULL, sizeof(struct rpcap_sockaddr), NULL,
 				    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
 					goto error;
 				daemon_seraddr((struct sockaddr_storage *) address->broadaddr, sockaddr);
 
 				sockaddr = (struct rpcap_sockaddr *) &sendbuf[sendbufidx];
 				if (sock_bufferize(NULL, sizeof(struct rpcap_sockaddr), NULL,
 				    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
 					goto error;
 				daemon_seraddr((struct sockaddr_storage *) address->dstaddr, sockaddr);
 				break;
 
 			default:
 				break;
 			}
 		}
 	}
 
 	pcap_freealldevs(alldevs);
 
 	if (sock_send(pars->sockctrl, sendbuf, sendbufidx, errbuf, PCAP_ERRBUF_SIZE) == -1)
 	{
 		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
 		return -1;
 	}
 
 	return 0;
 
 error:
 	if (alldevs)
 		pcap_freealldevs(alldevs);
 
 	if (rpcap_senderror(pars->sockctrl, ver, PCAP_ERR_FINDALLIF,
 	    errmsgbuf, errbuf) == -1)
 	{
 		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
 		return -1;
 	}
 	return 0;
 }