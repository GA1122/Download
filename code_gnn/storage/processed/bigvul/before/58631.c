BOOL rdp_read_share_data_header(STREAM* s, UINT16* length, BYTE* type, UINT32* share_id,
					BYTE *compressed_type, UINT16 *compressed_len)
{
	if (stream_get_left(s) < 12)
		return FALSE;

	 
	stream_read_UINT32(s, *share_id);  
	stream_seek_BYTE(s);  
	stream_seek_BYTE(s);  
	stream_read_UINT16(s, *length);  
	stream_read_BYTE(s, *type);  
	stream_read_BYTE(s, *compressed_type);  
	stream_read_UINT16(s, *compressed_len);  
	return TRUE;
}
