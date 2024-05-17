void rdp_write_share_data_header(STREAM* s, UINT16 length, BYTE type, UINT32 share_id)
{
	length -= RDP_PACKET_HEADER_MAX_LENGTH;
	length -= RDP_SHARE_CONTROL_HEADER_LENGTH;
	length -= RDP_SHARE_DATA_HEADER_LENGTH;

	 
	stream_write_UINT32(s, share_id);  
	stream_write_BYTE(s, 0);  
	stream_write_BYTE(s, STREAM_LOW);  
	stream_write_UINT16(s, length);  
	stream_write_BYTE(s, type);  
	stream_write_BYTE(s, 0);  
	stream_write_UINT16(s, 0);  
}
