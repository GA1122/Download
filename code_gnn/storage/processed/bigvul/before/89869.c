GetTotalBytesReceived(struct upnphttp * h, const char * action, const char * ns)
{
	int r;

	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewTotalBytesReceived>%lu</NewTotalBytesReceived>"
		"</u:%sResponse>";

	char body[512];
	int bodylen;
	struct ifdata data;

	r = getifstats(ext_if_name, &data);
	 
	bodylen = snprintf(body, sizeof(body), resp,
	         action, ns,  
#ifdef UPNP_STRICT
	         r<0?0:(data.ibytes & UPNP_UI4_MAX), action);
#else  
	         r<0?0:data.ibytes, action);
#endif  
	BuildSendAndCloseSoapResp(h, body, bodylen);
}