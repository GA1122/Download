int udf_CS0toUTF8(struct ustr *utf_o, const struct ustr *ocu_i)
{
	const uint8_t *ocu;
	uint8_t cmp_id, ocu_len;
	int i;

	ocu_len = ocu_i->u_len;
	if (ocu_len == 0) {
		memset(utf_o, 0, sizeof(struct ustr));
		return 0;
	}

	cmp_id = ocu_i->u_cmpID;
	if (cmp_id != 8 && cmp_id != 16) {
		memset(utf_o, 0, sizeof(struct ustr));
		pr_err("unknown compression code (%d) stri=%s\n",
		       cmp_id, ocu_i->u_name);
		return 0;
	}

	ocu = ocu_i->u_name;
	utf_o->u_len = 0;
	for (i = 0; (i < ocu_len) && (utf_o->u_len <= (UDF_NAME_LEN - 3));) {

		 
		uint32_t c = ocu[i++];
		if (cmp_id == 16)
			c = (c << 8) | ocu[i++];

		 
		if (c < 0x80U)
			utf_o->u_name[utf_o->u_len++] = (uint8_t)c;
		else if (c < 0x800U) {
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0xc0 | (c >> 6));
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0x80 | (c & 0x3f));
		} else {
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0xe0 | (c >> 12));
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0x80 |
							  ((c >> 6) & 0x3f));
			utf_o->u_name[utf_o->u_len++] =
						(uint8_t)(0x80 | (c & 0x3f));
		}
	}
	utf_o->u_cmpID = 8;

	return utf_o->u_len;
}