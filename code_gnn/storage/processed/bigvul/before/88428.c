daemon_msg_open_req(uint8 ver, struct daemon_slpars *pars, uint32 plen,
    char *source, size_t sourcelen)
{
	char errbuf[PCAP_ERRBUF_SIZE];		 
	char errmsgbuf[PCAP_ERRBUF_SIZE];	 
	pcap_t *fp;				 
	int nread;
	char sendbuf[RPCAP_NETBUF_SIZE];	 
	int sendbufidx = 0;			 
	struct rpcap_openreply *openreply;	 

	if (plen > sourcelen - 1)
	{
		pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Source string too long");
		goto error;
	}

	nread = sock_recv(pars->sockctrl, source, plen,
	    SOCK_RECEIVEALL_YES|SOCK_EOF_IS_ERROR, errbuf, PCAP_ERRBUF_SIZE);
	if (nread == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Read from client failed: %s", errbuf);
		return -1;
	}
	source[nread] = '\0';
	plen -= nread;

	if (is_url(source))
	{
		pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Source string refers to a remote device");
		goto error;
	}

	if ((fp = pcap_open_live(source,
			1500  ,
			0  ,
			1000  ,
			errmsgbuf)) == NULL)
		goto error;

	if (sock_bufferize(NULL, sizeof(struct rpcap_header), NULL, &sendbufidx,
	    RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	rpcap_createhdr((struct rpcap_header *) sendbuf, ver,
	    RPCAP_MSG_OPEN_REPLY, 0, sizeof(struct rpcap_openreply));

	openreply = (struct rpcap_openreply *) &sendbuf[sendbufidx];

	if (sock_bufferize(NULL, sizeof(struct rpcap_openreply), NULL, &sendbufidx,
	    RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	memset(openreply, 0, sizeof(struct rpcap_openreply));
	openreply->linktype = htonl(pcap_datalink(fp));
	openreply->tzoff = 0;  

	pcap_close(fp);

	if (sock_send(pars->sockctrl, sendbuf, sendbufidx, errbuf, PCAP_ERRBUF_SIZE) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}
	return 0;

error:
	if (rpcap_senderror(pars->sockctrl, ver, PCAP_ERR_OPEN,
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
}
