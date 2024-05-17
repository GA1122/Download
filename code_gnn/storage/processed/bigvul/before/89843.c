upnp_redirect_internal(const char * rhost, unsigned short eport,
                       const char * iaddr, unsigned short iport,
                       int proto, const char * desc,
                       unsigned int timestamp)
{
	 
	if(disable_port_forwarding)
		return -1;
	if(add_redirect_rule2(ext_if_name, rhost, eport, iaddr, iport, proto,
	                      desc, timestamp) < 0) {
		return -1;
	}

#ifdef ENABLE_LEASEFILE
	lease_file_add( eport, iaddr, iport, proto, desc, timestamp);
#endif
 
	if(add_filter_rule2(ext_if_name, rhost, iaddr, eport, iport, proto, desc) < 0) {
		 
#if !defined(__linux__)
		delete_redirect_rule(ext_if_name, eport, proto);
#endif
		return -1;
	}
	if(timestamp > 0) {
		if(!nextruletoclean_timestamp || (timestamp < nextruletoclean_timestamp))
			nextruletoclean_timestamp = timestamp;
	}
#ifdef ENABLE_EVENTS
	 
	upnp_event_var_change_notify(EWanIPC);
#endif
	return 0;
}
