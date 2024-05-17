evdns_request_data_build(const char *const name, const size_t name_len,
    const u16 trans_id, const u16 type, const u16 class,
    u8 *const buf, size_t buf_len) {
	off_t j = 0;   
	u16 t_;	  

	APPEND16(trans_id);
	APPEND16(0x0100);   
	APPEND16(1);   
	APPEND16(0);   
	APPEND16(0);   
	APPEND16(0);   

	j = dnsname_to_labels(buf, buf_len, j, name, name_len, NULL);
	if (j < 0) {
		return (int)j;
	}

	APPEND16(type);
	APPEND16(class);

	return (int)j;
 overflow:
	return (-1);
}
