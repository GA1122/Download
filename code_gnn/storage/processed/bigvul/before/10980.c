PHP_FUNCTION(xml_error_string)
{
	long code;
	char *str;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &code) == FAILURE) {
		return;
	}

	str = (char *)XML_ErrorString((int)code);
	if (str) {
		RETVAL_STRING(str, 1);
	}
 }
