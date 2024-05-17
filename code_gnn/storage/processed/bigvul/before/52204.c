static inline size_t php_mb2_int_to_char(unsigned char *buf, unsigned k)
{
	assert(k <= 0xFFFFU);
	 
	if (k <= 0xFFU) {  
		buf[0] = k;
		return 1U;
	} else {  
		buf[0] = k >> 8;
		buf[1] = k & 0xFFU;
		return 2U;
	}
}
