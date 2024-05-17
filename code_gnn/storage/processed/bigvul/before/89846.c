AddPinhole(struct upnphttp * h, const char * action, const char * ns)
{
	int r;
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<UniqueID>%d</UniqueID>"
		"</u:%sResponse>";
	char body[512];
	int bodylen;
	struct NameValueParserData data;
	char * rem_host, * rem_port, * int_ip, * int_port, * protocol, * leaseTime;
	int uid = 0;
	unsigned short iport, rport;
	int ltime;
	long proto;
	char rem_ip[INET6_ADDRSTRLEN];

	if(CheckStatus(h)==0)
		return;

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	rem_host = GetValueFromNameValueList(&data, "RemoteHost");
	rem_port = GetValueFromNameValueList(&data, "RemotePort");
	int_ip = GetValueFromNameValueList(&data, "InternalClient");
	int_port = GetValueFromNameValueList(&data, "InternalPort");
	protocol = GetValueFromNameValueList(&data, "Protocol");
	leaseTime = GetValueFromNameValueList(&data, "LeaseTime");

	rport = (unsigned short)(rem_port ? atoi(rem_port) : 0);
	iport = (unsigned short)(int_port ? atoi(int_port) : 0);
	ltime = leaseTime ? atoi(leaseTime) : -1;
	errno = 0;
	proto = protocol ? strtol(protocol, NULL, 0) : -1;
	if(errno != 0 || proto > 65535 || proto < 0)
	{
		SoapError(h, 402, "Invalid Args");
		goto clear_and_exit;
	}
	if(iport == 0)
	{
		SoapError(h, 706, "InternalPortWilcardingNotAllowed");
		goto clear_and_exit;
	}

	 
	if(!int_ip || int_ip[0] == '\0' || 0 == strcmp(int_ip, "*"))
	{
		SoapError(h, 708, "WildCardNotPermittedInSrcIP");
		goto clear_and_exit;
	}
	 
	if(rem_host)
	{
		 
		while(isspace(rem_host[0]))
			rem_host++;
	}
	 
	if(rem_host && (rem_host[0] != '\0') && (rem_host[0] != '*'))
	{
		struct addrinfo *ai, *p;
		struct addrinfo hints;
		int err;
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET6;
		 
		 
		err = getaddrinfo(rem_host, rem_port, &hints, &ai);
		if(err == 0)
		{
			 
			for(p = ai; p; p = p->ai_next)
			{
				if(p->ai_family == AF_INET6)
				{
					inet_ntop(AF_INET6,
					          &(((struct sockaddr_in6 *)p->ai_addr)->sin6_addr),
					          rem_ip, sizeof(rem_ip));
					syslog(LOG_INFO, "resolved '%s' to '%s'", rem_host, rem_ip);
					rem_host = rem_ip;
					break;
				}
			}
			freeaddrinfo(ai);
		}
		else
		{
			syslog(LOG_WARNING, "AddPinhole : getaddrinfo(%s) : %s",
			       rem_host, gai_strerror(err));
#if 0
			SoapError(h, 402, "Invalid Args");
			goto clear_and_exit;
#endif
		}
	}

	if(proto == 65535)
	{
		SoapError(h, 707, "ProtocolWilcardingNotAllowed");
		goto clear_and_exit;
	}
	if(proto != IPPROTO_UDP && proto != IPPROTO_TCP
#ifdef IPPROTO_UDPITE
	   && atoi(protocol) != IPPROTO_UDPLITE
#endif
	  )
	{
		SoapError(h, 705, "ProtocolNotSupported");
		goto clear_and_exit;
	}
	if(ltime < 1 || ltime > 86400)
	{
		syslog(LOG_WARNING, "%s: LeaseTime=%d not supported, (ip=%s)",
		       action, ltime, int_ip);
		SoapError(h, 402, "Invalid Args");
		goto clear_and_exit;
	}

	if(PinholeVerification(h, int_ip, iport) <= 0)
		goto clear_and_exit;

	syslog(LOG_INFO, "%s: (inbound) from [%s]:%hu to [%s]:%hu with proto %ld during %d sec",
	       action, rem_host?rem_host:"any",
	       rport, int_ip, iport,
	       proto, ltime);

	 
	r = upnp_add_inboundpinhole(rem_host, rport, int_ip, iport, proto, "IGD2 pinhole", ltime, &uid);

	switch(r)
	{
		case 1:	         
			bodylen = snprintf(body, sizeof(body),
			                   resp, action,
			                   ns ,
			                   uid, action);
			BuildSendAndCloseSoapResp(h, body, bodylen);
			break;
		case -1: 	 
			SoapError(h, 701, "PinholeSpaceExhausted");
			break;
		default:
			SoapError(h, 501, "ActionFailed");
			break;
	}
	 
clear_and_exit:
	ClearNameValueList(&data);
}
