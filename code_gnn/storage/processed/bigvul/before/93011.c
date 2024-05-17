rdp_out_control_caps(STREAM s)
{
	out_uint16_le(s, RDP_CAPSET_CONTROL);
	out_uint16_le(s, RDP_CAPLEN_CONTROL);

	out_uint16(s, 0);	 
	out_uint16(s, 0);	 
	out_uint16_le(s, 2);	 
	out_uint16_le(s, 2);	 
}
