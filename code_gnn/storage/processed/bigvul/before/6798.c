extract_cookie_value(char *hdr, const char *hdr_end,
		  char *cookie_name, size_t cookie_name_l, int list,
		  char **value, int *value_l)
{
	char *equal, *att_end, *att_beg, *val_beg, *val_end;
	char *next;

	 
	for (att_beg = hdr; att_beg + cookie_name_l + 1 < hdr_end; att_beg = next + 1) {
		 

		while (att_beg < hdr_end && HTTP_IS_SPHT(*att_beg))
			att_beg++;

		 
		equal = att_end = att_beg;

		while (equal < hdr_end) {
			if (*equal == '=' || *equal == ';' || (list && *equal == ','))
				break;
			if (HTTP_IS_SPHT(*equal++))
				continue;
			att_end = equal;
		}

		 

		 
		if (equal < hdr_end && *equal == '=') {
			 
			val_beg = equal + 1;
			while (val_beg < hdr_end && HTTP_IS_SPHT(*val_beg))
				val_beg++;

			 
			next = find_cookie_value_end(val_beg, hdr_end);

			 
			val_end = next;
			while (val_end > val_beg && HTTP_IS_SPHT(*(val_end - 1)))
				val_end--;
		} else {
			val_beg = val_end = next = equal;
		}

		 
		if (*att_beg == '$')
			continue;

		 
		if (equal == next)
			continue;

		 

		if (att_end - att_beg == cookie_name_l &&
		    memcmp(att_beg, cookie_name, cookie_name_l) == 0) {
			 
			*value = val_beg;
			*value_l = val_end - val_beg;
			return next + 1;
		}

		 
		if (!list)
			break;
	}

	return NULL;
}
