reverse(u8 *out, size_t outlen, const u8 *in, size_t inlen)
{
	if (inlen > outlen)
		return SC_ERROR_BUFFER_TOO_SMALL;
	outlen = inlen;
	while (inlen--)
		*out++ = in[inlen];
	return outlen;
}
