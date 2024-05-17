SendSSDPMSEARCHResponse(int s, const struct sockaddr * sockname,
                        const char * st, const char * usn,
                        const char * server, const char * location)
{
	int l, n;
	char buf[512];
	socklen_t sockname_len;
	 
	l = snprintf(buf, sizeof(buf), "HTTP/1.1 200 OK\r\n"
		"CACHE-CONTROL: max-age=120\r\n"
		 
		"ST: %s\r\n"
		"USN: %s\r\n"
		"EXT:\r\n"
		"SERVER: %s\r\n"
		"LOCATION: %s\r\n"
		"OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"  
		"01-NLS: %u\r\n"  
		"BOOTID.UPNP.ORG: %u\r\n"  
		"CONFIGID.UPNP.ORG: %u\r\n"  
		"\r\n",
		st, usn,
		server, location,
		upnp_bootid, upnp_bootid, upnp_configid);
#ifdef ENABLE_IPV6
	sockname_len = (sockname->sa_family == PF_INET6)
	             ? sizeof(struct sockaddr_in6)
	             : sizeof(struct sockaddr_in);
#else
	sockname_len = sizeof(struct sockaddr_in);
#endif
	n = sendto(s, buf, l, 0,
	           sockname, sockname_len );
	if(n < 0) {
		 
		syslog(LOG_ERR, "sendto(udp): %m");
	}
}
