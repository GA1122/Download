LogL16fromY(double Y, int em)	 
{
	if (Y >= 1.8371976e19)
		return (0x7fff);
	if (Y <= -1.8371976e19)
		return (0xffff);
	if (Y > 5.4136769e-20)
		return itrunc(256.*(log2(Y) + 64.), em);
	if (Y < -5.4136769e-20)
		return (~0x7fff | itrunc(256.*(log2(-Y) + 64.), em));
	return (0);
}