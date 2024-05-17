static void fips_expand_key_bits(BYTE* in, BYTE* out)
{
	BYTE buf[21], c;
	int i, b, p, r;

	 
	for (i = 0; i < 21; i++)
		buf[i] = fips_reverse_table[in[i]];

	 
	for (i = 0, b = 0; i < 24; i++, b += 7)
	{
		p = b / 8;
		r = b % 8;
		if (r == 0)
		{
			out[i] = buf[p] & 0xfe;
		}
		else
		{
			 
			c = buf[p] << r;
			c |= buf[p + 1] >> (8 - r);
			out[i] = c & 0xfe;
		}
	}

	 
	 
	for (i = 0; i < 24; i++)
		out[i] = fips_oddparity_table[fips_reverse_table[out[i]]];
}
