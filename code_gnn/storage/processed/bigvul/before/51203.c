isVDSetTerminator(struct iso9660 *iso9660, const unsigned char *h)
{
	(void)iso9660;  

	 
	if (h[0] != 255)
		return (0);

	 
	if (h[6] != 1)
		return (0);

	 
	if (!isNull(iso9660, h, 7, 2048-7))
		return (0);

	return (1);
}
