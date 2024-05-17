cliprdr_send_packet(uint16 type, uint16 status, uint8 * data, uint32 length)
{
	STREAM s;

	logger(Clipboard, Debug, "cliprdr_send_packet(), type=%d, status=%d, length=%d", type,
	       status, length);

	s = channel_init(cliprdr_channel, length + 12);
	out_uint16_le(s, type);
	out_uint16_le(s, status);
	out_uint32_le(s, length);
	out_uint8p(s, data, length);
	out_uint32(s, 0);	 
	s_mark_end(s);
	channel_send(s, cliprdr_channel);
}