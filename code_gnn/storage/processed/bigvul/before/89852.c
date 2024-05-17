DeletePortMapping(struct upnphttp * h, const char * action, const char * ns)
{
	int r;

	 
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"</u:%sResponse>";

	char body[512];
	int bodylen;
	struct NameValueParserData data;
	const char * ext_port, * protocol;
	unsigned short eport;
#ifdef UPNP_STRICT
	const char * r_host;
#endif  

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	ext_port = GetValueFromNameValueList(&data, "NewExternalPort");
	protocol = GetValueFromNameValueList(&data, "NewProtocol");
#ifdef UPNP_STRICT
	r_host = GetValueFromNameValueList(&data, "NewRemoteHost");
#endif  

#ifdef UPNP_STRICT
	if(!ext_port || !protocol || !r_host)
#else
	if(!ext_port || !protocol)
#endif  
	{
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

	eport = (unsigned short)atoi(ext_port);
	if(eport == 0)
	{
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}

	syslog(LOG_INFO, "%s: external port: %hu, protocol: %s",
		action, eport, protocol);

	 
	if(GETFLAG(SECUREMODEMASK))
	{
		char int_ip[32];
		struct in_addr int_ip_addr;
		unsigned short iport;
		unsigned int leaseduration = 0;
		r = upnp_get_redirection_infos(eport, protocol, &iport,
		                               int_ip, sizeof(int_ip),
		                               NULL, 0, NULL, 0,
		                               &leaseduration);
		if(r >= 0)
		{
			if(inet_pton(AF_INET, int_ip, &int_ip_addr) > 0)
			{
				if(h->clientaddr.s_addr != int_ip_addr.s_addr)
				{
					SoapError(h, 606, "Action not authorized");
					 
					ClearNameValueList(&data);
					return;
				}
			}
		}
	}

	r = upnp_delete_redirection(eport, protocol);

	if(r < 0)
	{
		SoapError(h, 714, "NoSuchEntryInArray");
	}
	else
	{
		bodylen = snprintf(body, sizeof(body), resp,
		                   action, ns, action);
		BuildSendAndCloseSoapResp(h, body, bodylen);
	}

	ClearNameValueList(&data);
}
