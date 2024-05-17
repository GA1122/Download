 upnp_redirect(const char * rhost, unsigned short eport,
               const char * iaddr, unsigned short iport,
               const char * protocol, const char * desc,
               unsigned int leaseduration)
 {
 	int proto, r;
 	char iaddr_old[32];
 	char rhost_old[32];
 	unsigned short iport_old;
 	struct in_addr address;
 	unsigned int timestamp;
 
 	proto = proto_atoi(protocol);
 	if(inet_aton(iaddr, &address) <= 0) {
 		syslog(LOG_ERR, "inet_aton(%s) FAILED", iaddr);
 		return -1;
 	}
 
 	if(!check_upnp_rule_against_permissions(upnppermlist, num_upnpperm,
 	                                        eport, address, iport)) {
 		syslog(LOG_INFO, "redirection permission check failed for "
  		                 "%hu->%s:%hu %s", eport, iaddr, iport, protocol);
  		return -3;
  	}
// 
// 	if (desc == NULL)
// 		desc = "";	 
// 
  	 
 	rhost_old[0] = '\0';
 	r = get_redirect_rule(ext_if_name, eport, proto,
 	                      iaddr_old, sizeof(iaddr_old), &iport_old, 0, 0,
 	                      rhost_old, sizeof(rhost_old),
 	                      &timestamp, 0, 0);
 	if(r == 0) {
 		if(strcmp(iaddr, iaddr_old)==0 &&
 		   ((rhost == NULL && rhost_old[0]=='\0') ||
 		    (rhost && (strcmp(rhost, "*") == 0) && rhost_old[0]=='\0') ||
 		    (rhost && (strcmp(rhost, rhost_old) == 0)))) {
 			syslog(LOG_INFO, "updating existing port mapping %hu %s (rhost '%s') => %s:%hu",
 				eport, protocol, rhost_old, iaddr_old, iport_old);
 			timestamp = (leaseduration > 0) ? upnp_time() + leaseduration : 0;
 			if(iport != iport_old) {
 				r = update_portmapping(ext_if_name, eport, proto, iport, desc, timestamp);
 			} else {
 				r = update_portmapping_desc_timestamp(ext_if_name, eport, proto, desc, timestamp);
 			}
 #ifdef ENABLE_LEASEFILE
 			if(r == 0) {
 				lease_file_remove(eport, proto);
 				lease_file_add(eport, iaddr, iport, proto, desc, timestamp);
 			}
 #endif  
 			return r;
 		} else {
 			syslog(LOG_INFO, "port %hu %s (rhost '%s') already redirected to %s:%hu",
 				eport, protocol, rhost_old, iaddr_old, iport_old);
 			return -2;
 		}
 #ifdef CHECK_PORTINUSE
 	} else if (port_in_use(ext_if_name, eport, proto, iaddr, iport) > 0) {
 		syslog(LOG_INFO, "port %hu protocol %s already in use",
 		       eport, protocol);
 		return -4;
 #endif  
 	} else {
 		timestamp = (leaseduration > 0) ? upnp_time() + leaseduration : 0;
 		syslog(LOG_INFO, "redirecting port %hu to %s:%hu protocol %s for: %s",
 			eport, iaddr, iport, protocol, desc);
 		return upnp_redirect_internal(rhost, eport, iaddr, iport, proto,
 		                              desc, timestamp);
 	}
 }