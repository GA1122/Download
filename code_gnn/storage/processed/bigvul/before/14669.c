PHPAPI zend_string *php_addcslashes(zend_string *str, int should_free, char *what, size_t wlength)
{
	char flags[256];
	char *source, *target;
	char *end;
	char c;
	size_t  newlen;
	zend_string *new_str = zend_string_alloc(4 * ZSTR_LEN(str), 0);

	php_charmask((unsigned char *)what, wlength, flags);

	for (source = (char*)ZSTR_VAL(str), end = source + ZSTR_LEN(str), target = ZSTR_VAL(new_str); source < end; source++) {
		c = *source;
		if (flags[(unsigned char)c]) {
			if ((unsigned char) c < 32 || (unsigned char) c > 126) {
				*target++ = '\\';
				switch (c) {
					case '\n': *target++ = 'n'; break;
					case '\t': *target++ = 't'; break;
					case '\r': *target++ = 'r'; break;
					case '\a': *target++ = 'a'; break;
					case '\v': *target++ = 'v'; break;
					case '\b': *target++ = 'b'; break;
					case '\f': *target++ = 'f'; break;
					default: target += sprintf(target, "%03o", (unsigned char) c);
				}
				continue;
			}
			*target++ = '\\';
		}
		*target++ = c;
	}
	*target = 0;
	newlen = target - ZSTR_VAL(new_str);
	if (newlen < ZSTR_LEN(str) * 4) {
		new_str = zend_string_truncate(new_str, newlen, 0);
	}
	if (should_free) {
		zend_string_release(str);
	}
	return new_str;
}
