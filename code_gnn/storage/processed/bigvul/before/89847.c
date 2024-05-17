AddPortMapping(struct upnphttp * h, const char * action, const char * ns)
{
	int r;

	 
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\"/>";

	char body[512];
	int bodylen;
	struct NameValueParserData data;
	char * int_ip, * int_port, * ext_port, * protocol, * desc;
	char * leaseduration_str;
	unsigned int leaseduration;
	char * r_host;
	unsigned short iport, eport;

	struct hostent *hp;  
	char ** ptr;  
	struct in_addr result_ip;   

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	int_ip = GetValueFromNameValueList(&data, "NewInternalClient");
	if (int_ip) {
		 
		while(int_ip[0] == ' ')
			int_ip++;
	}
#ifdef UPNP_STRICT
	if (!int_ip || int_ip[0] == '\0')
	{
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}
#endif

	 
	r_host = GetValueFromNameValueList(&data, "NewRemoteHost");
#ifndef SUPPORT_REMOTEHOST
#ifdef UPNP_STRICT
	if (r_host && (r_host[0] != '\0') && (0 != strcmp(r_host, "*")))
	{
		ClearNameValueList(&data);
		SoapError(h, 726, "RemoteHostOnlySupportsWildcard");
		return;
	}
#endif
#endif

#ifndef UPNP_STRICT
	 
	if (!int_ip || int_ip[0] == '\0')
	{
		int_ip = h->clientaddr_str;
		memcpy(&result_ip, &(h->clientaddr), sizeof(struct in_addr));
	}
	else
#endif
	 
	if (inet_pton(AF_INET, int_ip, &result_ip) <= 0)
	{
		hp = gethostbyname(int_ip);
		if(hp && hp->h_addrtype == AF_INET)
		{
			for(ptr = hp->h_addr_list; ptr && *ptr; ptr++)
		   	{
				int_ip = inet_ntoa(*((struct in_addr *) *ptr));
				result_ip = *((struct in_addr *) *ptr);
				 
				break;
			}
		}
		else
		{
			syslog(LOG_ERR, "Failed to convert hostname '%s' to ip address", int_ip);
			ClearNameValueList(&data);
			SoapError(h, 402, "Invalid Args");
			return;
		}
	}

	 
	if(GETFLAG(SECUREMODEMASK))
	{
		if(h->clientaddr.s_addr != result_ip.s_addr)
		{
			syslog(LOG_INFO, "Client %s tried to redirect port to %s",
			       inet_ntoa(h->clientaddr), int_ip);
			ClearNameValueList(&data);
			SoapError(h, 718, "ConflictInMappingEntry");
			return;
		}
	}

	int_port = GetValueFromNameValueList(&data, "NewInternalPort");
	ext_port = GetValueFromNameValueList(&data, "NewExternalPort");
	protocol = GetValueFromNameValueList(&data, "NewProtocol");
	desc = GetValueFromNameValueList(&data, "NewPortMappingDescription");
	leaseduration_str = GetValueFromNameValueList(&data, "NewLeaseDuration");

	if (!int_port || !ext_port || !protocol)
	{
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}

	eport = (unsigned short)atoi(ext_port);
	iport = (unsigned short)atoi(int_port);

	if (strcmp(ext_port, "*") == 0 || eport == 0)
	{
		ClearNameValueList(&data);
		SoapError(h, 716, "Wildcard not permited in ExtPort");
		return;
	}

	leaseduration = leaseduration_str ? atoi(leaseduration_str) : 0;
#ifdef IGD_V2
	 
	if(leaseduration == 0 || leaseduration > 604800)
		leaseduration = 604800;
#endif

	syslog(LOG_INFO, "%s: ext port %hu to %s:%hu protocol %s for: %s leaseduration=%u rhost=%s",
	       action, eport, int_ip, iport, protocol, desc, leaseduration,
	       r_host ? r_host : "NULL");

	r = upnp_redirect(r_host, eport, int_ip, iport, protocol, desc, leaseduration);

	ClearNameValueList(&data);

	 
	switch(r)
	{
	case 0:	 
		bodylen = snprintf(body, sizeof(body), resp,
		                   action, ns );
		BuildSendAndCloseSoapResp(h, body, bodylen);
		break;
	case -4:
#ifdef IGD_V2
		SoapError(h, 729, "ConflictWithOtherMechanisms");
		break;
#endif  
	case -2:	 
	case -3:	 
		SoapError(h, 718, "ConflictInMappingEntry");
		break;
	default:
		SoapError(h, 501, "ActionFailed");
	}
}
