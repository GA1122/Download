rdp_out_bmpcache_caps(STREAM s)
{
	int Bpp;

	logger(Protocol, Debug, "%s()", __func__);

	out_uint16_le(s, RDP_CAPSET_BMPCACHE);
	out_uint16_le(s, RDP_CAPLEN_BMPCACHE);

	Bpp = (g_server_depth + 7) / 8;	 
	out_uint8s(s, 24);	 
	out_uint16_le(s, 0x258);	 
	out_uint16_le(s, 0x100 * Bpp);	 
	out_uint16_le(s, 0x12c);	 
	out_uint16_le(s, 0x400 * Bpp);	 
	out_uint16_le(s, 0x106);	 
	out_uint16_le(s, 0x1000 * Bpp);	 
}
