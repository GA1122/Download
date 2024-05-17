rdp_send_fonts(uint16 seq)
{
	STREAM s;

	logger(Protocol, Debug, "%s()", __func__);

	s = rdp_init_data(8);

	out_uint16(s, 0);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, seq);	 
	out_uint16_le(s, 0x32);	 

	s_mark_end(s);
	rdp_send_data(s, RDP_DATA_PDU_FONT2);
}
