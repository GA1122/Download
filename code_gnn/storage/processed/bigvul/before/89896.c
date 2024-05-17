processMSEARCH(int s, const char * st, size_t st_len,
               const struct sockaddr * addr)
{
	struct service * serv;
#ifdef ENABLE_IPV6
	char buf[64];
#endif  

	if(!st || st_len==0)
		return;
#ifdef ENABLE_IPV6
	sockaddr_to_string(addr, buf, sizeof(buf));
	syslog(LOG_INFO, "SSDP M-SEARCH from %s ST:%.*s",
	       buf, (int)st_len, st);
#else	 
	syslog(LOG_INFO, "SSDP M-SEARCH from %s:%d ST: %.*s",
	       inet_ntoa(((const struct sockaddr_in *)addr)->sin_addr),
	       ntohs(((const struct sockaddr_in *)addr)->sin_port),
	       (int)st_len, st);
#endif	 
	if(st_len==8 && (0==memcmp(st, "ssdp:all", 8))) {
		 
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			SendSSDPMSEARCHResponse(s, addr,
			                        serv->st, strlen(serv->st), serv->usn,
			                        serv->server, serv->location);
		}
	} else if(st_len > 5 && (0==memcmp(st, "uuid:", 5))) {
		 
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			if(0 == strncmp(serv->usn, st, st_len)) {
				SendSSDPMSEARCHResponse(s, addr,
				                        serv->st, strlen(serv->st), serv->usn,
				                        serv->server, serv->location);
			}
		}
	} else {
		size_t l;
		int st_ver = 0;
		char atoi_buffer[8];

		 
		for (l = st_len; l > 0; l--) {
			if (st[l-1] == ':') {
				memset(atoi_buffer, 0, sizeof(atoi_buffer));
				memcpy(atoi_buffer, st + l, MIN((sizeof(atoi_buffer) - 1), st_len - l));
				st_ver = atoi(atoi_buffer);
				break;
			}
		}
		if (l == 0)
			l = st_len;
		 
		 
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			if(0 == strncmp(serv->st, st, l)) {
				syslog(LOG_DEBUG, "Found matching service : %s %s", serv->st, serv->location);
				SendSSDPMSEARCHResponse(s, addr,
				                        st, st_len, serv->usn,
				                        serv->server, serv->location);
			}
		}
	}
}
