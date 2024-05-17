rdpdr_send_client_announce_reply(void)
{
	 
	STREAM s;
	s = channel_init(rdpdr_channel, 12);
	out_uint16_le(s, RDPDR_CTYP_CORE);
	out_uint16_le(s, PAKID_CORE_CLIENTID_CONFIRM);
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 5);	 
	out_uint32_be(s, g_client_id);	 
	s_mark_end(s);
	channel_send(s, rdpdr_channel);
}
