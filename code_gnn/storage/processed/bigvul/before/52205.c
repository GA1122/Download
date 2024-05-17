static inline size_t php_mb3_int_to_char(unsigned char *buf, unsigned k)
{
	assert(k <= 0xFFFFFFU);
	 
	if (k <= 0xFFU) {  
		buf[0] = k;
		return 1U;
	} else if (k <= 0xFFFFU) {  
		buf[0] = k >> 8;
		buf[1] = k & 0xFFU;
		return 2U;
	} else {
		buf[0] = k >> 16;
		buf[1] = (k >> 8) & 0xFFU;
		buf[2] = k & 0xFFU;
		return 3U;
	}
}
