mcs_send_edrq(void)
{
	STREAM s;
	logger(Protocol, Debug, "%s()", __func__);
	s = iso_init(5);

	out_uint8(s, (MCS_EDRQ << 2));
	out_uint16_be(s, 1);	 
	out_uint16_be(s, 1);	 

	s_mark_end(s);
	iso_send(s);
}