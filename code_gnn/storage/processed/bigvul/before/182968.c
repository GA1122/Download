 cliprdr_process(STREAM s)
 {
  	uint16 type, status;
  	uint32 length, format;
  	uint8 *data;
// 	struct stream packet = *s;
  
  	in_uint16_le(s, type);
  	in_uint16_le(s, status);
 	in_uint32_le(s, length);
 	data = s->p;
 
  	logger(Clipboard, Debug, "cliprdr_process(), type=%d, status=%d, length=%d", type, status,
  	       length);
  
// 	if (!s_check_rem(s, length))
// 	{
// 		rdp_protocol_error("cliprdr_process(), consume of packet from stream would overrun", &packet);
// 	}
// 
  	if (status == CLIPRDR_ERROR)
  	{
  		switch (type)
 		{
 			case CLIPRDR_FORMAT_ACK:
 				 
 				cliprdr_send_native_format_announce(last_formats,
 								    last_formats_length);
 				break;
 			case CLIPRDR_DATA_RESPONSE:
 				ui_clip_request_failed();
 				break;
 			default:
 				logger(Clipboard, Warning,
 				       "cliprdr_process(), unhandled error (type=%d)", type);
 		}
 
 		return;
 	}
 
 	switch (type)
 	{
 		case CLIPRDR_CONNECT:
 			ui_clip_sync();
 			break;
 		case CLIPRDR_FORMAT_ANNOUNCE:
 			ui_clip_format_announce(data, length);
 			cliprdr_send_packet(CLIPRDR_FORMAT_ACK, CLIPRDR_RESPONSE, NULL, 0);
 			return;
 		case CLIPRDR_FORMAT_ACK:
 			break;
 		case CLIPRDR_DATA_REQUEST:
 			in_uint32_le(s, format);
 			ui_clip_request_data(format);
 			break;
 		case CLIPRDR_DATA_RESPONSE:
 			ui_clip_handle_data(data, length);
 			break;
 		case 7:	 
 			break;
 		default:
 			logger(Clipboard, Warning, "cliprdr_process(), unhandled packet type %d",
 			       type);
 	}
 }