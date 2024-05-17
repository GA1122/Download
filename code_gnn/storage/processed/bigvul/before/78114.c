static int encode_bit_string(const u8 * inbuf, size_t bits_left, u8 **outbuf,
			     size_t *outlen, int invert)
{
	const u8 *in = inbuf;
	u8 *out;
	size_t bytes;
	int skipped = 0;

	bytes = (bits_left + 7)/8 + 1;
	*outbuf = out = malloc(bytes);
	if (out == NULL)
		return SC_ERROR_OUT_OF_MEMORY;
	*outlen = bytes;
	out += 1;
	while (bits_left) {
		int i, bits_to_go = 8;

		*out = 0;
		if (bits_left < 8) {
			bits_to_go = bits_left;
			skipped = 8 - bits_left;
		}
		if (invert) {
			for (i = 0; i < bits_to_go; i++)
				*out |= ((*in >> i) & 1) << (7 - i);
		} else {
			*out = *in;
			if (bits_left < 8)
				return SC_ERROR_NOT_SUPPORTED;  
		}
		bits_left -= bits_to_go;
		out++, in++;
	}
	out = *outbuf;
	out[0] = skipped;
	return 0;
}