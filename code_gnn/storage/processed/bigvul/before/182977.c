 rdpdr_process(STREAM s)
 {
 	uint32 handle;
  	uint16 vmin;
  	uint16 component;
  	uint16 pakid;
// 	struct stream packet = *s;
  
  	logger(Protocol, Debug, "rdpdr_process()");
  	 
 
 	in_uint16(s, component);
 	in_uint16(s, pakid);
 
 	if (component == RDPDR_CTYP_CORE)
 	{
 		switch (pakid)
 		{
 			case PAKID_CORE_DEVICE_IOREQUEST:
 				rdpdr_process_irp(s);
 				break;
 
 			case PAKID_CORE_SERVER_ANNOUNCE:
  				 
  				in_uint8s(s, 2);	 
  				in_uint16_le(s, vmin);	 
// 
  				in_uint32_le(s, g_client_id);	 
  
// 				 
// 				if (!s_check(s))
// 				{
// 					rdp_protocol_error("rdpdr_process(), consume of g_client_id from stream did overrun", &packet);
// 				}
// 
  				 
  				if (vmin < 0x000c)
 					g_client_id = 0x815ed39d;	 
 				g_epoch++;
 
 #if WITH_SCARD
 				 
 				scard_release_all_contexts();
 
 				 
 
 #endif
 
 				rdpdr_send_client_announce_reply();
 				rdpdr_send_client_name_request();
 				break;
 
 			case PAKID_CORE_CLIENTID_CONFIRM:
 				rdpdr_send_client_device_list_announce();
 				break;
 
 			case PAKID_CORE_DEVICE_REPLY:
 				in_uint32(s, handle);
 				logger(Protocol, Debug,
 				       "rdpdr_process(), server connected to resource %d", handle);
 				break;
 
 			case PAKID_CORE_SERVER_CAPABILITY:
 				rdpdr_send_client_capability_response();
 				break;
 
 			default:
 				logger(Protocol, Debug,
 				       "rdpdr_process(), pakid 0x%x of component 0x%x", pakid,
 				       component);
 				break;
 
 		}
 	}
 	else if (component == RDPDR_CTYP_PRN)
 	{
 		if (pakid == PAKID_PRN_CACHE_DATA)
 			printercache_process(s);
 	}
 	else
 		logger(Protocol, Warning, "rdpdr_process(), unhandled component 0x%x", component);
 }