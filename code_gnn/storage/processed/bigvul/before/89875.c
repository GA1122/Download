SendSetupMessage(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<OutMessage>%s</OutMessage>"
		"</u:%sResponse>";
	char body[1024];
	int bodylen;
	struct NameValueParserData data;
	const char * ProtocolType;	 
	const char * InMessage;		 
	const char * OutMessage = "";	 

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	ProtocolType = GetValueFromNameValueList(&data, "ProtocolType");	 
	InMessage = GetValueFromNameValueList(&data, "InMessage");	 

	if(ProtocolType == NULL || InMessage == NULL)
	{
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}
	 
	if(strcmp(ProtocolType, "WPS") != 0)
	{
		ClearNameValueList(&data);
		SoapError(h, 600, "Argument Value Invalid");  
		return;
	}
	 

	bodylen = snprintf(body, sizeof(body), resp,
	                   action, ns ,
	                   OutMessage, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
	ClearNameValueList(&data);
}
