static ZEND_RESULT_CODE to_utf16(parse_mb_func fn, const char *u8, uint16_t **u16, size_t *len TSRMLS_DC)
{
	size_t offset = 0, u8_len = strlen(u8);

	*u16 = ecalloc(4 * sizeof(uint16_t), u8_len + 1);
	*len = 0;

	while (offset < u8_len) {
		unsigned wc;
		uint16_t buf[2], *ptr = buf;
		size_t consumed = fn(&wc, &u8[offset], &u8[u8_len]);

		if (!consumed) {
			efree(*u16);
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to parse UTF-8 at pos %zu of '%s'", offset, u8);
			return FAILURE;
		} else {
			offset += consumed;
		}

		switch (wctoutf16(buf, wc)) {
		case 2:
			(*u16)[(*len)++] = *ptr++;
			 
		case 1:
			(*u16)[(*len)++] = *ptr++;
			break;
		case 0:
		default:
			efree(*u16);
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to convert UTF-32 'U+%X' to UTF-16", wc);
			return FAILURE;
		}
	}

	return SUCCESS;
}