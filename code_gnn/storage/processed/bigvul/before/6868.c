void init_proto_http()
{
	int i;
	char *tmp;
	int msg;

	for (msg = 0; msg < HTTP_ERR_SIZE; msg++) {
		if (!http_err_msgs[msg]) {
			ha_alert("Internal error: no message defined for HTTP return code %d. Aborting.\n", msg);
			abort();
		}

		http_err_chunks[msg].str = (char *)http_err_msgs[msg];
		http_err_chunks[msg].len = strlen(http_err_msgs[msg]);
	}

	 
	memset(hdr_encode_map, 0, sizeof(hdr_encode_map));
	memset(url_encode_map, 0, sizeof(url_encode_map));
	memset(http_encode_map, 0, sizeof(url_encode_map));
	for (i = 0; i < 32; i++) {
		FD_SET(i, hdr_encode_map);
		FD_SET(i, url_encode_map);
	}
	for (i = 127; i < 256; i++) {
		FD_SET(i, hdr_encode_map);
		FD_SET(i, url_encode_map);
	}

	tmp = "\"#{|}";
	while (*tmp) {
		FD_SET(*tmp, hdr_encode_map);
		tmp++;
	}

	tmp = "\"#";
	while (*tmp) {
		FD_SET(*tmp, url_encode_map);
		tmp++;
	}

	 
	memset(http_encode_map, 0, sizeof(http_encode_map));
	for (i = 0x00; i <= 0x08; i++)
		FD_SET(i, http_encode_map);
	for (i = 0x0a; i <= 0x1f; i++)
		FD_SET(i, http_encode_map);
	FD_SET(0x7f, http_encode_map);

	 
	pool_head_http_txn = create_pool("http_txn", sizeof(struct http_txn), MEM_F_SHARED);
	pool_head_uniqueid = create_pool("uniqueid", UNIQUEID_LEN, MEM_F_SHARED);
}