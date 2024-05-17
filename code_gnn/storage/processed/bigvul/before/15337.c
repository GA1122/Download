static void php_xmlwriter_string_arg(INTERNAL_FUNCTION_PARAMETERS, xmlwriter_read_one_char_t internal_function, char *err_string)
{
	zval *pind;
	xmlwriter_object *intern;
	xmlTextWriterPtr ptr;
	char *name;
	int name_len, retval;

#ifdef ZEND_ENGINE_2
	zval *this = getThis();
	
	if (this) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE) {
			return;
		}
		XMLWRITER_FROM_OBJECT(intern, this);
	} else
#endif
	{
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &pind, &name, &name_len) == FAILURE) {
			return;
		}
	
		ZEND_FETCH_RESOURCE(intern,xmlwriter_object *, &pind, -1, "XMLWriter", le_xmlwriter);
	}

	if (err_string != NULL) {
		XMLW_NAME_CHK(err_string);
	}

	ptr = intern->ptr;

	if (ptr) {
		retval = internal_function(ptr, (xmlChar *) name);
		if (retval != -1) {
			RETURN_TRUE;
		}
	}
	
	RETURN_FALSE;
}
