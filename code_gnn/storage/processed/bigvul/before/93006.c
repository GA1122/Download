rdp_out_activate_caps(STREAM s)
{
	out_uint16_le(s, RDP_CAPSET_ACTIVATE);
	out_uint16_le(s, RDP_CAPLEN_ACTIVATE);

	out_uint16(s, 0);	 
	out_uint16(s, 0);	 
	out_uint16(s, 0);	 
	out_uint16(s, 0);	 
}
