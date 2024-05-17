AddAnyPortMapping(struct upnphttp * h, const char * action, const char * ns)
{
	int r;
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewReservedPort>%hu</NewReservedPort>"
		"</u:%sResponse>";

	char body[512];
	int bodylen;

	struct NameValueParserData data;
	const char * int_ip, * int_port, * ext_port, * protocol, * desc;
	const char * r_host;
	unsigned short iport, eport;
	const char * leaseduration_str;
	unsigned int leaseduration;

	struct hostent *hp;  
	char ** ptr;  
	struct in_addr result_ip;   

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	r_host = GetValueFromNameValueList(&data, "NewRemoteHost");
	ext_port = GetValueFromNameValueList(&data, "NewExternalPort");
	protocol = GetValueFromNameValueList(&data, "NewProtocol");
	int_port = GetValueFromNameValueList(&data, "NewInternalPort");
	int_ip = GetValueFromNameValueList(&data, "NewInternalClient");
	 
	desc = GetValueFromNameValueList(&data, "NewPortMappingDescription");
	leaseduration_str = GetValueFromNameValueList(&data, "NewLeaseDuration");

	leaseduration = leaseduration_str ? atoi(leaseduration_str) : 0;
	if(leaseduration == 0)
		leaseduration = 604800;

	if (!int_ip || !ext_port || !int_port)
	{
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}

	eport = (unsigned short)atoi(ext_port);
	iport = (unsigned short)atoi(int_port);
	if(iport == 0 || !is_numeric(ext_port)) {
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}
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
			SoapError(h, 606, "Action not authorized");
			return;
		}
	}

	 
	for(;;) {
		r = upnp_redirect(r_host, eport, int_ip, iport, protocol, desc, leaseduration);
		if(r==-2 && eport < 65535) {
			eport++;
		} else {
			break;
		}
	}

	ClearNameValueList(&data);

	switch(r)
	{
	case 0:	 
		bodylen = snprintf(body, sizeof(body), resp,
		              action, ns,  
					  eport, action);
		BuildSendAndCloseSoapResp(h, body, bodylen);
		break;
	case -2:	 
		SoapError(h, 718, "ConflictInMappingEntry");
		break;
	case -3:	 
		SoapError(h, 606, "Action not authorized");
		break;
	default:
		SoapError(h, 501, "ActionFailed");
	}
}
