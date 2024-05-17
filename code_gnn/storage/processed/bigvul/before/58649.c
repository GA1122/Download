void rdp_write_security_header(STREAM* s, UINT16 flags)
{
	 
	stream_write_UINT16(s, flags);  
	stream_write_UINT16(s, 0);  
}
