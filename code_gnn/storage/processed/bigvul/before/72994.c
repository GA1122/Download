int jpc_bitstream_needalign(jpc_bitstream_t *bitstream)
{
	if (bitstream->openmode_ & JPC_BITSTREAM_READ) {
		 
		 
		if ((bitstream->cnt_ < 8 && bitstream->cnt_ > 0) ||
		  ((bitstream->buf_ >> 8) & 0xff) == 0xff) {
			return 1;
		}
	} else if (bitstream->openmode_ & JPC_BITSTREAM_WRITE) {
		 
		 
		if ((bitstream->cnt_ < 8 && bitstream->cnt_ >= 0) ||
		  ((bitstream->buf_ >> 8) & 0xff) == 0xff) {
			return 1;
		}
	} else {
		 
		assert(0);
		return -1;
	}
	return 0;
}
