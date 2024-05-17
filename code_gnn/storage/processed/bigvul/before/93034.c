rdp_send_control(uint16 action)
{
	STREAM s;

	s = rdp_init_data(8);

	out_uint16_le(s, action);
	out_uint16(s, 0);	 
	out_uint32(s, 0);	 

	s_mark_end(s);
	rdp_send_data(s, RDP_DATA_PDU_CONTROL);
}
