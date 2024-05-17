find_url_param_pos(const char **chunks,
                   const char* url_param_name, size_t url_param_name_l,
                   char delim)
{
	const char *pos, *last, *equal;
	const char **bufs = chunks;
	int l1, l2;


	pos  = bufs[0];
	last = bufs[1];
	while (pos < last) {
		 
		equal = pos + url_param_name_l;
		if (fix_pointer_if_wrap(chunks, &equal)) {
			if (equal >= chunks[3])
				return NULL;
		} else {
			if (equal >= chunks[1])
				return NULL;
		}
		if (*equal == '=') {
			if (pos + url_param_name_l > last) {
				 

				 
				if (bufs != chunks || !bufs[2])
					return NULL;

				 
				l1 = last - pos;
				l2 = url_param_name_l - l1;

				 
				if (bufs[2] + l2 > bufs[3])
					return NULL;

				if (memcmp(pos,     url_param_name,    l1) == 0 &&
				    memcmp(bufs[2], url_param_name+l1, l2) == 0)
					return pos;

				 
				bufs += 2;
				pos = bufs[0] + l2;
				last = bufs[1];

			} else {
				 
				if (memcmp(pos, url_param_name, url_param_name_l) == 0)
					return pos;
				pos += url_param_name_l + 1;
				if (fix_pointer_if_wrap(chunks, &pos))
					last = bufs[2];
			}
		}

		while (1) {
			 
			while (pos < last && !is_param_delimiter(*pos, delim))
				pos++;
			if (pos < last)
				break;
			 
			if (bufs != chunks || !bufs[2])
				return NULL;
			bufs += 2;
			pos = bufs[0];
			last = bufs[1];
		}
		pos++;
	}
	return NULL;
}
