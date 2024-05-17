PHPAPI zend_string *php_addslashes(zend_string *str, int should_free)
{
	 
	char *source, *target;
	char *end;
	size_t offset;
	zend_string *new_str;

	if (!str) {
		return ZSTR_EMPTY_ALLOC();
	}

	source = ZSTR_VAL(str);
	end = source + ZSTR_LEN(str);

	while (source < end) {
		switch (*source) {
			case '\0':
			case '\'':
			case '\"':
			case '\\':
				goto do_escape;
			default:
				source++;
				break;
		}
	}

	if (!should_free) {
		return zend_string_copy(str);
	}

	return str;

do_escape:
	offset = source - (char *)ZSTR_VAL(str);
	new_str = zend_string_alloc(offset +  (2 * (ZSTR_LEN(str) - offset)), 0);
	memcpy(ZSTR_VAL(new_str), ZSTR_VAL(str), offset);
	target = ZSTR_VAL(new_str) + offset;

	while (source < end) {
		switch (*source) {
			case '\0':
				*target++ = '\\';
				*target++ = '0';
				break;
			case '\'':
			case '\"':
			case '\\':
				*target++ = '\\';
				 
			default:
				*target++ = *source;
				break;
		}

		source++;
	}

	*target = 0;
	if (should_free) {
		zend_string_release(str);
	}

	if (ZSTR_LEN(new_str) - (target - ZSTR_VAL(new_str)) > 16) {
		new_str = zend_string_truncate(new_str, target - ZSTR_VAL(new_str), 0);
	} else {
		ZSTR_LEN(new_str) = target - ZSTR_VAL(new_str);
	}

	return new_str;
}
