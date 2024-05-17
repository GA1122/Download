static void php_xmlwriter_flush(INTERNAL_FUNCTION_PARAMETERS, int force_string) {
	zval *pind;
	xmlwriter_object *intern;
	xmlTextWriterPtr ptr;
	xmlBufferPtr buffer;
	zend_bool empty = 1;
	int output_bytes;


#ifdef ZEND_ENGINE_2
	zval *this = getThis();

	if (this) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &empty) == FAILURE) {
			return;
		}
		XMLWRITER_FROM_OBJECT(intern, this);
	} else
#endif
	{
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|b", &pind, &empty) == FAILURE) {
			return;
		}

		ZEND_FETCH_RESOURCE(intern,xmlwriter_object *, &pind, -1, "XMLWriter", le_xmlwriter);
	}
	ptr = intern->ptr;

	if (ptr) {
		buffer = intern->output;
		if (force_string == 1 && buffer == NULL) {
			RETURN_EMPTY_STRING();
		}
		output_bytes = xmlTextWriterFlush(ptr);
		if (buffer) {
			RETVAL_STRING((char *) buffer->content, 1);
			if (empty) {
				xmlBufferEmpty(buffer);
			}
		} else {
			RETVAL_LONG(output_bytes);
		}
		return;
	}
	
	RETURN_EMPTY_STRING();
}
