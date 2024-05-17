ParseSSDPPacket(int s, const char * p, ssize_t n,
                const struct sockaddr * addr,
                const char * searched_device)
{
	const char * linestart;
	const char * lineend;
	const char * nameend;
	const char * valuestart;
	struct header headers[3];
	int i, r = 0;
	int methodlen;
	int nts = -1;
	int method = -1;
	unsigned int lifetime = 180;	 
	const char * st = NULL;
	int st_len = 0;

	 
	if(get_lan_for_peer(addr) == NULL) {
		char addr_str[64];
		sockaddr_to_string(addr, addr_str, sizeof(addr_str));
		syslog(LOG_WARNING, "peer %s is not from a LAN",
		       addr_str);
		return 0;
	}

	 
	memset(headers, 0, sizeof(headers));
	for(methodlen = 0;
	    methodlen < n && (isalpha(p[methodlen]) || p[methodlen]=='-');
		methodlen++);
	if(methodlen==8 && 0==memcmp(p, "M-SEARCH", 8))
		method = METHOD_MSEARCH;
	else if(methodlen==6 && 0==memcmp(p, "NOTIFY", 6))
		method = METHOD_NOTIFY;
	else if(methodlen==4 && 0==memcmp(p, "HTTP", 4)) {
		 
		method = METHOD_NOTIFY;
		nts = NTS_SSDP_ALIVE;
	}
	linestart = p;
	while(linestart < p + n - 2) {
		 
		lineend = linestart;
		while(lineend < p + n && *lineend != '\n' && *lineend != '\r')
			lineend++;
		 
		 
		nameend = linestart;
		while(nameend < lineend && *nameend != ':')
			nameend++;
		 
		if(nameend < lineend)
			valuestart = nameend + 1;
		else
			valuestart = nameend;
		 
		while(valuestart < lineend && isspace(*valuestart))
			valuestart++;
		 
		if(valuestart < lineend && *valuestart=='\"')
			valuestart++;
		if(nameend > linestart && valuestart < lineend) {
			int l = nameend - linestart;	 
			int m = lineend - valuestart;	 
			 
			while(m>0 && isspace(valuestart[m-1]))
				m--;
			 
			if(m>0 && valuestart[m-1] == '\"')
				m--;
			i = -1;
			 
			if(l==2 && 0==strncasecmp(linestart, "nt", 2))
				i = HEADER_NT;
			else if(l==3 && 0==strncasecmp(linestart, "usn", 3))
				i = HEADER_USN;
			else if(l==3 && 0==strncasecmp(linestart, "nts", 3)) {
				if(m==10 && 0==strncasecmp(valuestart, "ssdp:alive", 10))
					nts = NTS_SSDP_ALIVE;
				else if(m==11 && 0==strncasecmp(valuestart, "ssdp:byebye", 11))
					nts = NTS_SSDP_BYEBYE;
				else if(m==11 && 0==strncasecmp(valuestart, "ssdp:update", 11))
					nts = NTS_SSDP_UPDATE;
			}
			else if(l==8 && 0==strncasecmp(linestart, "location", 8))
				i = HEADER_LOCATION;
			else if(l==13 && 0==strncasecmp(linestart, "cache-control", 13)) {
				 
				const char * name = valuestart;	 
				const char * val;				 
				int rem = m;	 
				while(rem > 0) {
					val = name;
					while(val < name + rem && *val != '=' && *val != ',')
						val++;
					if(val >= name + rem)
						break;
					if(*val == '=') {
						while(val < name + rem && (*val == '=' || isspace(*val)))
							val++;
						if(val >= name + rem)
							break;
						if(0==strncasecmp(name, "max-age", 7))
							lifetime = (unsigned int)strtoul(val, 0, 0);
						 
						while(rem > 0 && *name != ',') {
							rem--;
							name++;
						}
						 
						while(rem > 0 && (*name == ',' || isspace(*name))) {
							rem--;
							name++;
						}
					} else {
						rem -= (val - name);
						name = val;
						while(rem > 0 && (*name == ',' || isspace(*name))) {
							rem--;
							name++;
						}
					}
				}
				 
			} else if(l==2 && 0==strncasecmp(linestart, "st", 2)) {
				st = valuestart;
				st_len = m;
				if(method == METHOD_NOTIFY)
					i = HEADER_NT;	 
			}
			if(i>=0) {
				headers[i].p = valuestart;
				headers[i].l = m;
			}
		}
		linestart = lineend;
		while((linestart < p + n) && (*linestart == '\n' || *linestart == '\r'))
			linestart++;
	}
#if 0
	printf("NTS=%d\n", nts);
	for(i=0; i<3; i++) {
		if(headers[i].p)
			printf("%d-'%.*s'\n", i, headers[i].l, headers[i].p);
	}
#endif
	syslog(LOG_DEBUG,"SSDP request: '%.*s' (%d) %s %s=%.*s",
	       methodlen, p, method, nts_to_str(nts),
	       (method==METHOD_NOTIFY)?"nt":"st",
	       (method==METHOD_NOTIFY)?headers[HEADER_NT].l:st_len,
	       (method==METHOD_NOTIFY)?headers[HEADER_NT].p:st);
	switch(method) {
	case METHOD_NOTIFY:
		if(nts==NTS_SSDP_ALIVE || nts==NTS_SSDP_UPDATE) {
			if(headers[HEADER_NT].p && headers[HEADER_USN].p && headers[HEADER_LOCATION].p) {
				 
				if(searched_device &&
				   0 != memcmp(headers[HEADER_NT].p, searched_device, headers[HEADER_NT].l))
					break;
				r = updateDevice(headers, time(NULL) + lifetime);
			} else {
				syslog(LOG_WARNING, "missing header nt=%p usn=%p location=%p",
				       headers[HEADER_NT].p, headers[HEADER_USN].p,
				       headers[HEADER_LOCATION].p);
			}
		} else if(nts==NTS_SSDP_BYEBYE) {
			if(headers[HEADER_NT].p && headers[HEADER_USN].p) {
				r = removeDevice(headers);
			} else {
				syslog(LOG_WARNING, "missing header nt=%p usn=%p",
				       headers[HEADER_NT].p, headers[HEADER_USN].p);
			}
		}
		break;
	case METHOD_MSEARCH:
		processMSEARCH(s, st, st_len, addr);
		break;
	default:
		{
			char addr_str[64];
			sockaddr_to_string(addr, addr_str, sizeof(addr_str));
			syslog(LOG_WARNING, "method %.*s, don't know what to do (from %s)",
			       methodlen, p, addr_str);
		}
	}
	return r;
}
