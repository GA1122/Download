lha_crc16(uint16_t crc, const void *pp, size_t len)
{
	const unsigned char *p = (const unsigned char *)pp;
	const uint16_t *buff;
	const union {
		uint32_t i;
		char c[4];
	} u = { 0x01020304 };

	if (len == 0)
		return crc;

	 
	if (((uintptr_t)p) & (uintptr_t)0x1) {
		crc = (crc >> 8) ^ crc16tbl[0][(crc ^ *p++) & 0xff];
		len--;
	}
	buff = (const uint16_t *)p;
	 
	for (;len >= 8; len -= 8) {
		 
#undef bswap16
#if defined(_MSC_VER) && _MSC_VER >= 1400   
#  define bswap16(x) _byteswap_ushort(x)
#elif defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 8) || __GNUC__ > 4)
 
#  define bswap16(x) __builtin_bswap16(x)
#elif defined(__clang__)
 
#  define bswap16(x) __builtin_bswap16(x)
#else
#  define bswap16(x) ((((x) >> 8) & 0xff) | ((x) << 8))
#endif
#define CRC16W	do { 	\
		if(u.c[0] == 1) {  		\
			crc ^= bswap16(*buff); buff++;		\
		} else						\
			crc ^= *buff++;				\
		crc = crc16tbl[1][crc & 0xff] ^ crc16tbl[0][crc >> 8];\
} while (0)
		CRC16W;
		CRC16W;
		CRC16W;
		CRC16W;
#undef CRC16W
#undef bswap16
	}

	p = (const unsigned char *)buff;
	for (;len; len--) {
		crc = (crc >> 8) ^ crc16tbl[0][(crc ^ *p++) & 0xff];
	}
	return crc;
}
