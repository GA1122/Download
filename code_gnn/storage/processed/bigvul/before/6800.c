char *find_hdr_value_end(char *s, const char *e)
{
	int quoted, qdpair;

	quoted = qdpair = 0;

#if defined(__x86_64__) ||						\
    defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || \
    defined(__ARM_ARCH_7A__)
	 
	for (; s <= e - sizeof(int); s += sizeof(int)) {
		unsigned int c = *(int *)s;  
		unsigned int q = c;          

		c ^= 0x2c2c2c2c;  
		q ^= 0x22222222;  

		c = (c - 0x01010101) & ~c;  
		q = (q - 0x01010101) & ~q;  

		if ((c | q) & 0x80808080)
			break;  
	}
#endif
	for (; s < e; s++) {
		if (qdpair)                    qdpair = 0;
		else if (quoted) {
			if (*s == '\\')        qdpair = 1;
			else if (*s == '"')    quoted = 0;
		}
		else if (*s == '"')            quoted = 1;
		else if (*s == ',')            return s;
	}
	return s;
}
