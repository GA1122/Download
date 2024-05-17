PHP_FUNCTION(parse_str)
{
	char *arg;
	zval *arrayArg = NULL;
	char *res = NULL;
	size_t arglen;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|z/", &arg, &arglen, &arrayArg) == FAILURE) {
		return;
	}

	res = estrndup(arg, arglen);

	if (arrayArg == NULL) {
		zval tmp;
		zend_array *symbol_table = zend_rebuild_symbol_table();

		ZVAL_ARR(&tmp, symbol_table);
		sapi_module.treat_data(PARSE_STRING, res, &tmp);
	} else 	{
		zval ret;

		 
		zval_dtor(arrayArg);
		array_init(&ret);
		sapi_module.treat_data(PARSE_STRING, res, &ret);
		ZVAL_COPY_VALUE(arrayArg, &ret);
	}
}
