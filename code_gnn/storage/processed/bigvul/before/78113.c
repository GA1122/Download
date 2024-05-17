static int encode_bit_field(const u8 *inbuf, size_t inlen,
			    u8 **outbuf, size_t *outlen)
{
	u8		data[sizeof(unsigned int)];
	unsigned int	field = 0;
	size_t		i, bits;

	if (inlen != sizeof(data))
		return SC_ERROR_BUFFER_TOO_SMALL;

	 
	memcpy(&field, inbuf, inlen);
	for (bits = 0; field; bits++)
		field >>= 1;

	memcpy(&field, inbuf, inlen);
	for (i = 0; i < bits; i += 8)
		data[i/8] = field >> i;

	return encode_bit_string(data, bits, outbuf, outlen, 1);
}