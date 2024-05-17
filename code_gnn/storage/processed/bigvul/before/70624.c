evdns_request_len(const size_t name_len) {
	return 96 +  
		name_len + 2 +
		4;   
}
