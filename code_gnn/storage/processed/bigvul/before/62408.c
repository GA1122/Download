get_data_src_dst_mac(uint16_t fc, const u_char *p, const uint8_t **srcp,
                     const uint8_t **dstp)
{
#define ADDR1  (p + 4)
#define ADDR2  (p + 10)
#define ADDR3  (p + 16)
#define ADDR4  (p + 24)

	if (!FC_TO_DS(fc)) {
		if (!FC_FROM_DS(fc)) {
			 
			*srcp = ADDR2;
			*dstp = ADDR1;
		} else {
			 
			*srcp = ADDR3;
			*dstp = ADDR1;
		}
	} else {
		if (!FC_FROM_DS(fc)) {
			 
			*srcp = ADDR2;
			*dstp = ADDR3;
		} else {
			 
			*srcp = ADDR4;
			*dstp = ADDR3;
		}
	}

#undef ADDR1
#undef ADDR2
#undef ADDR3
#undef ADDR4
}