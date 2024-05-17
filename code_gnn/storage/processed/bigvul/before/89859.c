GetDefaultConnectionService(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
#ifdef IGD_V2
		"<NewDefaultConnectionService>%s:WANConnectionDevice:2,"
#else
		"<NewDefaultConnectionService>%s:WANConnectionDevice:1,"
#endif
		SERVICE_ID_WANIPC "</NewDefaultConnectionService>"
		"</u:%sResponse>";
	 
	char body[1024];
	int bodylen;

	 
	bodylen = snprintf(body, sizeof(body), resp,
	                   action, ns, uuidvalue_wcd, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
