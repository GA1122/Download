int jpc_bitstream_outalign(jpc_bitstream_t *bitstream, int filldata)
{
	int n;
	int v;

	 
	assert(bitstream->openmode_ & JPC_BITSTREAM_WRITE);

	 
	 
	assert(!(filldata & (~0x3f)));

	if (!bitstream->cnt_) {
		if ((bitstream->buf_ & 0xff) == 0xff) {
			n = 7;
			v = filldata;
		} else {
			n = 0;
			v = 0;
		}
	} else if (bitstream->cnt_ > 0 && bitstream->cnt_ < 8) {
		n = bitstream->cnt_;
		v = filldata >> (7 - n);
	} else {
		n = 0;
		v = 0;
		return 0;
	}

	 
	if (n > 0) {
		if (jpc_bitstream_putbits(bitstream, n, v)) {
			return -1;
		}
	}
	if (bitstream->cnt_ < 8) {
		assert(bitstream->cnt_ >= 0 && bitstream->cnt_ < 8);
		assert((bitstream->buf_ & 0xff) != 0xff);
		 
		if (jas_stream_putc(bitstream->stream_, bitstream->buf_ & 0xff) == EOF) {
			return -1;
		}
		bitstream->cnt_ = 8;
		bitstream->buf_ = (bitstream->buf_ << 8) & 0xffff;
	}

	return 0;
}
