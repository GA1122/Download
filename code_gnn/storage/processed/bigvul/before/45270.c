static int udf_UTF8toCS0(dstring *ocu, struct ustr *utf, int length)
{
	unsigned c, i, max_val, utf_char;
	int utf_cnt, u_len;

	memset(ocu, 0, sizeof(dstring) * length);
	ocu[0] = 8;
	max_val = 0xffU;

try_again:
	u_len = 0U;
	utf_char = 0U;
	utf_cnt = 0U;
	for (i = 0U; i < utf->u_len; i++) {
		c = (uint8_t)utf->u_name[i];

		 
		if (utf_cnt) {
			utf_char = (utf_char << 6) | (c & 0x3fU);
			if (--utf_cnt)
				continue;
		} else {
			 
			if (c & 0x80U) {
				 
				if ((c & 0xe0U) == 0xc0U) {
					utf_char = c & 0x1fU;
					utf_cnt = 1;
				} else if ((c & 0xf0U) == 0xe0U) {
					utf_char = c & 0x0fU;
					utf_cnt = 2;
				} else if ((c & 0xf8U) == 0xf0U) {
					utf_char = c & 0x07U;
					utf_cnt = 3;
				} else if ((c & 0xfcU) == 0xf8U) {
					utf_char = c & 0x03U;
					utf_cnt = 4;
				} else if ((c & 0xfeU) == 0xfcU) {
					utf_char = c & 0x01U;
					utf_cnt = 5;
				} else {
					goto error_out;
				}
				continue;
			} else {
				 
				utf_char = c;
			}
		}

		 
		if (utf_char > max_val) {
			if (max_val == 0xffU) {
				max_val = 0xffffU;
				ocu[0] = (uint8_t)0x10U;
				goto try_again;
			}
			goto error_out;
		}

		if (max_val == 0xffffU)
			ocu[++u_len] = (uint8_t)(utf_char >> 8);
		ocu[++u_len] = (uint8_t)(utf_char & 0xffU);
	}

	if (utf_cnt) {
error_out:
		ocu[++u_len] = '?';
		printk(KERN_DEBUG pr_fmt("bad UTF-8 character\n"));
	}

	ocu[length - 1] = (uint8_t)u_len + 1;

	return u_len + 1;
}