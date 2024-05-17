GetNATRSIPStatus(struct upnphttp * h, const char * action, const char * ns)
{
#if 0
	static const char resp[] =
		"<u:GetNATRSIPStatusResponse "
		"xmlns:u=\"" SERVICE_TYPE_WANIPC "\">"
		"<NewRSIPAvailable>0</NewRSIPAvailable>"
		"<NewNATEnabled>1</NewNATEnabled>"
		"</u:GetNATRSIPStatusResponse>";
	UNUSED(action);
#endif
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewRSIPAvailable>0</NewRSIPAvailable>"
		"<NewNATEnabled>1</NewNATEnabled>"
		"</u:%sResponse>";
	char body[512];
	int bodylen;
	 
	bodylen = snprintf(body, sizeof(body), resp,
		action, ns,  
		action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
