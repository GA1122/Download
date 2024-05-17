void rdp_write_share_control_header(STREAM* s, UINT16 length, UINT16 type, UINT16 channel_id)
{
	length -= RDP_PACKET_HEADER_MAX_LENGTH;

	 
	stream_write_UINT16(s, length);  
	stream_write_UINT16(s, type | 0x10);  
	stream_write_UINT16(s, channel_id);  
}
