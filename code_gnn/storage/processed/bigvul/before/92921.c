cliprdr_send_data_request(uint32 format)
{
	uint8 buffer[4];

	logger(Clipboard, Debug, "cliprdr_send_data_request(), format 0x%x", format);
	buf_out_uint32(buffer, format);
	cliprdr_send_packet(CLIPRDR_DATA_REQUEST, CLIPRDR_REQUEST, buffer, sizeof(buffer));
}