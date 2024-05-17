isBootRecord(struct iso9660 *iso9660, const unsigned char *h)
{
	(void)iso9660;  

	 
	if (h[0] != 0)
		return (0);

	 
	if (h[6] != 1)
		return (0);

	return (1);
}
