rdp_out_ts_bitmap_capabilityset(STREAM s)
{
	logger(Protocol, Debug, "rdp_out_ts_bitmap_capabilityset(), %dx%d",
	       g_session_width, g_session_height);
	out_uint16_le(s, RDP_CAPSET_BITMAP);
	out_uint16_le(s, RDP_CAPLEN_BITMAP);
	out_uint16_le(s, g_server_depth);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, g_session_width);	 
	out_uint16_le(s, g_session_height);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 1);	 
	out_uint8(s, 0);	 
	out_uint8(s, 0);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 0);	 
}
