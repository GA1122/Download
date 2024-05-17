static unsigned get_bits(bunzip_data *bd, int bits_wanted)
{
	unsigned bits = 0;
	 
	int bit_count = bd->inbufBitCount;

	 
	while (bit_count < bits_wanted) {

		 
		if (bd->inbufPos == bd->inbufCount) {
			 
			bd->inbufCount = read(bd->in_fd, bd->inbuf, IOBUF_SIZE);
			if (bd->inbufCount <= 0)
				longjmp(bd->jmpbuf, RETVAL_UNEXPECTED_INPUT_EOF);
			bd->inbufPos = 0;
		}

		 
		if (bit_count >= 24) {
			bits = bd->inbufBits & ((1U << bit_count) - 1);
			bits_wanted -= bit_count;
			bits <<= bits_wanted;
			bit_count = 0;
		}

		 
		bd->inbufBits = (bd->inbufBits << 8) | bd->inbuf[bd->inbufPos++];
		bit_count += 8;
	}

	 
	bit_count -= bits_wanted;
	bd->inbufBitCount = bit_count;
	bits |= (bd->inbufBits >> bit_count) & ((1 << bits_wanted) - 1);

	return bits;
}
