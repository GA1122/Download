int jpc_bitstream_pending(jpc_bitstream_t *bitstream)
{
	if (bitstream->openmode_ & JPC_BITSTREAM_WRITE) {
		 
#if 1
		 
		if (bitstream->cnt_ < 8) {
			return 1;
		}
#else
		if (bitstream->cnt_ < 8) {
			if (((bitstream->buf_ >> 8) & 0xff) == 0xff) {
				return 2;
			}
			return 1;
		}
#endif
		return 0;
	} else {
		 
		return -1;
	}
}
