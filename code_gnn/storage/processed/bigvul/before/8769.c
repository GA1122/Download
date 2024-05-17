static int mton(uint32_t mask)
{
	int i = 0;
	mask = ntohl(mask);  
	while (mask) {
		i++;
		mask <<= 1;
	}
	return i;
}
