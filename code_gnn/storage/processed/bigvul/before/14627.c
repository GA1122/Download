PHP_FUNCTION(substr)
{
	zend_string *str;
	zend_long l = 0, f;
	int argc = ZEND_NUM_ARGS();

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sl|l", &str, &f, &l) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_STR(str)
		Z_PARAM_LONG(f)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(l)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (argc > 2) {
		if ((l < 0 && (size_t)(-l) > ZSTR_LEN(str))) {
			RETURN_FALSE;
		} else if (l > (zend_long)ZSTR_LEN(str)) {
			l = ZSTR_LEN(str);
		}
	} else {
		l = ZSTR_LEN(str);
	}

	if (f > (zend_long)ZSTR_LEN(str)) {
		RETURN_FALSE;
	} else if (f < 0 && -f > ZSTR_LEN(str)) {
		f = 0;
	}

	if (l < 0 && (l + (zend_long)ZSTR_LEN(str) - f) < 0) {
		RETURN_FALSE;
	}

	 
	if (f < 0) {
		f = (zend_long)ZSTR_LEN(str) + f;
		if (f < 0) {
			f = 0;
		}
	}

	 
	if (l < 0) {
		l = ((zend_long)ZSTR_LEN(str) - f) + l;
		if (l < 0) {
			l = 0;
		}
	}

	if (f > (zend_long)ZSTR_LEN(str)) {
		RETURN_FALSE;
	}

	if ((f + l) > (zend_long)ZSTR_LEN(str)) {
		l = ZSTR_LEN(str) - f;
	}

	RETURN_STRINGL(ZSTR_VAL(str) + f, l);
}
