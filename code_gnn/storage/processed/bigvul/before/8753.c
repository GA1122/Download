int FAST_FUNC sprint_nip6(char *dest,   const uint8_t *ip)
{
	char hexstrbuf[16 * 2];
	bin2hex(hexstrbuf, (void*)ip, 16);
	return sprintf(dest,  
		"%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s:%.4s",
		 
		hexstrbuf + 0 * 4,
		hexstrbuf + 1 * 4,
		hexstrbuf + 2 * 4,
		hexstrbuf + 3 * 4,
		hexstrbuf + 4 * 4,
		hexstrbuf + 5 * 4,
		hexstrbuf + 6 * 4,
		hexstrbuf + 7 * 4
	);
}
