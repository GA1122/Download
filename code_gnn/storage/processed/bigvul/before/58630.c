BOOL rdp_read_security_header(STREAM* s, UINT16* flags)
{
	 
	if(stream_get_left(s) < 4)
		return FALSE;
	stream_read_UINT16(s, *flags);  
	stream_seek(s, 2);  
	return TRUE;
}
