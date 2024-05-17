PHP_FUNCTION(similar_text)
{
	zend_string *t1, *t2;
	zval *percent = NULL;
	int ac = ZEND_NUM_ARGS();
	size_t sim;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS|z/", &t1, &t2, &percent) == FAILURE) {
		return;
	}

	if (ac > 2) {
		convert_to_double_ex(percent);
	}

	if (ZSTR_LEN(t1) + ZSTR_LEN(t2) == 0) {
		if (ac > 2) {
			Z_DVAL_P(percent) = 0;
		}

		RETURN_LONG(0);
	}

	sim = php_similar_char(ZSTR_VAL(t1), ZSTR_LEN(t1), ZSTR_VAL(t2), ZSTR_LEN(t2));

	if (ac > 2) {
		Z_DVAL_P(percent) = sim * 200.0 / (ZSTR_LEN(t1) + ZSTR_LEN(t2));
	}

	RETURN_LONG(sim);
}
