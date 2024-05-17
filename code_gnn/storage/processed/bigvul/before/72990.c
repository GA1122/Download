int jpc_bitstream_close(jpc_bitstream_t *bitstream)
{
	int ret = 0;

	 
	if (jpc_bitstream_align(bitstream)) {
		ret = -1;
	}

	 
	if (!(bitstream->flags_ & JPC_BITSTREAM_NOCLOSE) && bitstream->stream_) {
		if (jas_stream_close(bitstream->stream_)) {
			ret = -1;
		}
		bitstream->stream_ = 0;
	}

	jas_free(bitstream);
	return ret;
}