static int parse_q922_addr(netdissect_options *ndo,
                           const u_char *p, u_int *dlci,
                           u_int *addr_len, uint8_t *flags, u_int length)
{
	if (!ND_TTEST(p[0]) || length < 1)
		return -1;
	if ((p[0] & FR_EA_BIT))
		return 0;

	if (!ND_TTEST(p[1]) || length < 2)
		return -1;
	*addr_len = 2;
	*dlci = ((p[0] & 0xFC) << 2) | ((p[1] & 0xF0) >> 4);

        flags[0] = p[0] & 0x02;  
        flags[1] = p[1] & 0x0c;
        flags[2] = 0;            
        flags[3] = 0;

	if (p[1] & FR_EA_BIT)
		return 1;	 

	p += 2;
	length -= 2;
	if (!ND_TTEST(p[0]) || length < 1)
		return -1;
	(*addr_len)++;		 
	if ((p[0] & FR_EA_BIT) == 0) {
		*dlci = (*dlci << 7) | (p[0] >> 1);
		(*addr_len)++;	 
		p++;
		length--;
	}

	if (!ND_TTEST(p[0]) || length < 1)
		return -1;
	if ((p[0] & FR_EA_BIT) == 0)
		return 0;  

        flags[3] = p[0] & 0x02;

        *dlci = (*dlci << 6) | (p[0] >> 2);

	return 1;
}
