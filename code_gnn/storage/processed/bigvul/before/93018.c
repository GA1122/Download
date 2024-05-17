rdp_out_ts_general_capabilityset(STREAM s)
{
	uint16 extraFlags = 0;
	if (g_rdp_version >= RDP_V5)
	{
		extraFlags |= NO_BITMAP_COMPRESSION_HDR;
		extraFlags |= AUTORECONNECT_SUPPORTED;
		extraFlags |= LONG_CREDENTIALS_SUPPORTED;
		extraFlags |= FASTPATH_OUTPUT_SUPPORTED;
	}

	out_uint16_le(s, RDP_CAPSET_GENERAL);
	out_uint16_le(s, RDP_CAPLEN_GENERAL);
	out_uint16_le(s, OSMAJORTYPE_WINDOWS);	 
	out_uint16_le(s, OSMINORTYPE_WINDOWSNT);	 
	out_uint16_le(s, TS_CAPS_PROTOCOLVERSION);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, extraFlags);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 0);	 
	out_uint8(s, 0);	 
	out_uint8(s, 0);	 
}
