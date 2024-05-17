int ndp_msg_next_opt_offset(struct ndp_msg *msg, int offset,
			    enum ndp_msg_opt_type opt_type)
{
	unsigned char *opts_start = ndp_msg_payload_opts(msg);
	unsigned char *ptr = opts_start;
	size_t len = ndp_msg_payload_opts_len(msg);
	uint8_t opt_raw_type = ndp_msg_opt_type_info(opt_type)->raw_type;
	bool ignore = true;

	if (offset == -1) {
		offset = 0;
		ignore = false;
	}

	ptr += offset;
	len -= offset;
	while (len > 0) {
		uint8_t cur_opt_raw_type = ptr[0];
		unsigned int cur_opt_len = ptr[1] << 3;  

		if (!cur_opt_len || len < cur_opt_len)
			break;
		if (cur_opt_raw_type == opt_raw_type && !ignore)
			return ptr - opts_start;
		ptr += cur_opt_len;
		len -= cur_opt_len;
		ignore = false;
	}
	return -1;
}