 sec_recv(RD_BOOL * is_fastpath)
 {
 	uint8 fastpath_hdr, fastpath_flags;
  	uint16 sec_flags;
  	uint16 channel;
  	STREAM s;
// 	struct stream packet;
  
  	while ((s = mcs_recv(&channel, is_fastpath, &fastpath_hdr)) != NULL)
  	{
// 		packet = *s;
  		if (*is_fastpath == True)
  		{
  			 
 			 
  			fastpath_flags = (fastpath_hdr & 0xC0) >> 6;
  			if (fastpath_flags & FASTPATH_OUTPUT_ENCRYPTED)
  			{
// 				if (!s_check_rem(s, 8)) {
// 					rdp_protocol_error("sec_recv(), consume fastpath signature from stream would overrun", &packet);
// 				}
// 
  				in_uint8s(s, 8);	 
  				sec_decrypt(s->p, s->end - s->p);
  			}
 			return s;
 		}
 
 		if (g_encryption || (!g_licence_issued && !g_licence_error_result))
 		{
 			 
 			in_uint16_le(s, sec_flags);
 			in_uint8s(s, 2);	 
 
 			if (g_encryption)
  			{
  				if (sec_flags & SEC_ENCRYPT)
  				{
// 					if (!s_check_rem(s, 8)) {
// 						rdp_protocol_error("sec_recv(), consume encrypt signature from stream would overrun", &packet);
// 					}
// 
  					in_uint8s(s, 8);	 
  					sec_decrypt(s->p, s->end - s->p);
  				}
 
 				if (sec_flags & SEC_LICENSE_PKT)
 				{
 					licence_process(s);
 					continue;
 				}
 
 				if (sec_flags & SEC_REDIRECTION_PKT)
  				{
  					uint8 swapbyte;
  
// 					if (!s_check_rem(s, 8)) {
// 						rdp_protocol_error("sec_recv(), consume redirect signature from stream would overrun", &packet);
// 					}
// 
  					in_uint8s(s, 8);	 
  					sec_decrypt(s->p, s->end - s->p);
  
 					 
 					if (s->p[0] == 0 && s->p[1] == 4)
 					{
 						 
 						swapbyte = s->p[0];
 						s->p[0] = s->p[2];
 						s->p[2] = swapbyte;
 
 						swapbyte = s->p[1];
 						s->p[1] = s->p[3];
 						s->p[3] = swapbyte;
 
 						swapbyte = s->p[2];
 						s->p[2] = s->p[3];
 						s->p[3] = swapbyte;
 					}
 				}
 			}
 			else
 			{
 				if (sec_flags & SEC_LICENSE_PKT)
 				{
 					licence_process(s);
 					continue;
 				}
 				s->p -= 4;
 			}
 		}
 
 		if (channel != MCS_GLOBAL_CHANNEL)
 		{
 			channel_process(s, channel);
 			continue;
 		}
 
 		return s;
 	}
 
 	return NULL;
 }