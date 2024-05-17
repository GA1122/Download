static int ldm_parse_hexbyte (const u8 *src)
{
	unsigned int x;		 
	int h;

	 
	x = h = hex_to_bin(src[0]);
	if (h < 0)
		return -1;

	 
	h = hex_to_bin(src[1]);
	if (h < 0)
		return -1;

	return (x << 4) + h;
}
