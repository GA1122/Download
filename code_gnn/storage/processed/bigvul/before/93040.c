rdp_ts_in_share_control_header(STREAM s, uint8 * type, uint16 * length)
{
	uint16 pdu_type;
	uint16 pdu_source;

	UNUSED(pdu_source);

	in_uint16_le(s, *length);	 

	 
	if (*length == 0x8000)
	{
		 
		g_next_packet += 8;
		return False;
	}

	in_uint16_le(s, pdu_type);	 
	in_uint16(s, pdu_source);	 

	*type = pdu_type & 0xf;

	return True;
}
