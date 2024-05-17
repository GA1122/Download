static int find_highest_bits(int *dat)
{
	u32  bits, bitnum;
	int i;

	 
	for (i = 7; i >= 0 ; i--) {
		bits = dat[i];
		if (bits) {
			bitnum = fls(bits);
			return i * 32 + bitnum - 1;
		}
	}

	return -1;
}
