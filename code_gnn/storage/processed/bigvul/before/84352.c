UNCURL_EXPORT int32_t uncurl_ws_read(struct uncurl_conn *ucc, char *buf, uint32_t buf_len, uint8_t *opcode)
{
	int32_t e;
	char header_buf[WS_HEADER_SIZE];
	struct ws_header h;

	e = ucc->read(ucc->ctx, header_buf, 2);
	if (e != UNCURL_OK) return e;
	ws_parse_header0(&h, header_buf);
	*opcode = h.opcode;

	e = ucc->read(ucc->ctx, header_buf, h.addtl_bytes);
	if (e != UNCURL_OK) return e;
	ws_parse_header1(&h, header_buf);

	if (h.payload_len > ucc->opts.max_body || h.payload_len > INT32_MAX) return UNCURL_ERR_MAX_BODY;
	if (h.payload_len > buf_len) return UNCURL_ERR_BUFFER;

	e = ucc->read(ucc->ctx, buf, (uint32_t) h.payload_len);
	if (e != UNCURL_OK) return e;

	if (h.mask)
		ws_mask(buf, buf, h.payload_len, h.masking_key);

	return (int32_t) h.payload_len;
}
