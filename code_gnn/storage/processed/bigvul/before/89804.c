static void DeletePCPMap(pcp_info_t *pcp_msg_info)
{
	uint16_t iport = pcp_msg_info->int_port;   
	uint8_t  proto = pcp_msg_info->protocol;
	int r=-1;
	 
	 
	int index;
	unsigned short eport2, iport2;
	char iaddr2[INET6_ADDRSTRLEN];
	int proto2;
	char desc[64];
	unsigned int timestamp;
#ifdef ENABLE_UPNPPINHOLE
	int uid = -1;
#endif  

	 
	for (index = 0 ;
	     (!pcp_msg_info->is_fw &&
	      get_redirect_rule_by_index(index, 0,
					 &eport2, iaddr2, sizeof(iaddr2),
					 &iport2, &proto2,
					 desc, sizeof(desc),
					 0, 0, &timestamp, 0, 0) >= 0)
#ifdef ENABLE_UPNPPINHOLE
	       ||
	     (pcp_msg_info->is_fw &&
	      (uid=upnp_get_pinhole_uid_by_index(index))>=0 &&
	      upnp_get_pinhole_info((unsigned short)uid,
				    NULL, 0, NULL,
				    iaddr2, sizeof(iaddr2), &iport2,
				    &proto2, desc, sizeof(desc),
				    &timestamp, NULL) >= 0)
#endif  
		     ;
	     index++)
		if(0 == strcmp(iaddr2, pcp_msg_info->mapped_str)
		   && (proto2==proto)
		   && ((iport2==iport) || (iport==0))) {
			if(0 != strcmp(desc, pcp_msg_info->desc)) {
				 
				pcp_msg_info->result_code = PCP_ERR_NOT_AUTHORIZED;
				syslog(LOG_ERR, "Unauthorized to remove PCP mapping internal port %hu, protocol %s",
				       iport, (pcp_msg_info->protocol == IPPROTO_TCP)?"TCP":"UDP");
				return;
			} else if (!pcp_msg_info->is_fw) {
				r = _upnp_delete_redir(eport2, proto2);
			} else {
#ifdef ENABLE_UPNPPINHOLE
				r = upnp_delete_inboundpinhole(uid);
#endif  
			}
			break;
		}
	if (r >= 0) {
		syslog(LOG_INFO, "PCP: %s port %hu mapping removed",
		       proto2==IPPROTO_TCP?"TCP":"UDP", eport2);
	} else {
		syslog(LOG_ERR, "Failed to remove PCP mapping internal port %hu, protocol %s",
		       iport, (pcp_msg_info->protocol == IPPROTO_TCP)?"TCP":"UDP");
		pcp_msg_info->result_code = PCP_ERR_NO_RESOURCES;
	}
}
