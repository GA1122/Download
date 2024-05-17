gstd_read(void *the_tok, char *buf, int length)
{
	struct gstd_tok	*tok = the_tok;
	gss_buffer_desc	in;
	OM_uint32	maj, min;
	int		bufpos = tok->gstd_inbufpos;
	int		ret;

	 

	if (bufpos == -1 || bufpos >= tok->gstd_inbuf.length) {
		if (tok->gstd_inbuf.length > 0)
			gss_release_buffer(&min, &tok->gstd_inbuf);

		 
		ret = read_packet(tok->gstd_fd, &in, 0, 0);
		if (ret <= 0)
			return ret;

		maj = gss_unwrap(&min, tok->gstd_ctx, &in, &tok->gstd_inbuf,
		    NULL, NULL);
		if (maj != GSS_S_COMPLETE) {
			gstd_error(LOG_ERR, min, "gss_unwrap");
			return -1;
		}
		gss_release_buffer(&min, &in);
		bufpos = 0;
	}

	 

	length = MIN(length, tok->gstd_inbuf.length - bufpos);
	memcpy(buf, tok->gstd_inbuf.value + bufpos, length);
	tok->gstd_inbufpos = bufpos + length;
	LOG(LOG_DEBUG, ("read %d bytes", length));
	return length;
}
