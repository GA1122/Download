static void php_xmlwriter_end(INTERNAL_FUNCTION_PARAMETERS, xmlwriter_read_int_t internal_function)
{
	zval *pind;
	xmlwriter_object *intern;
	xmlTextWriterPtr ptr;
	int retval;
#ifdef ZEND_ENGINE_2
	zval *this = getThis();
	
	if (this) {
		XMLWRITER_FROM_OBJECT(intern, this);
		if (zend_parse_parameters_none() == FAILURE) {
			return;
		}
	} else 
#endif
	{
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &pind) == FAILURE) {
			return;
		}
		ZEND_FETCH_RESOURCE(intern,xmlwriter_object *, &pind, -1, "XMLWriter", le_xmlwriter);
	}

	ptr = intern->ptr;

	if (ptr) {
		retval = internal_function(ptr);
		if (retval != -1) {
			RETURN_TRUE;
		}
	}
	
	RETURN_FALSE;
}
