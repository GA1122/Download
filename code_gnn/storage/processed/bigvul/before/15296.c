static PHP_FUNCTION(xmlwriter_start_attribute_ns)
{
	zval *pind;
	xmlwriter_object *intern;
	xmlTextWriterPtr ptr;
	char *name, *prefix, *uri;
	int name_len, prefix_len, uri_len, retval;
#ifdef ZEND_ENGINE_2
	zval *this = getThis();
	
	if (this) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss!", 
			&prefix, &prefix_len, &name, &name_len, &uri, &uri_len) == FAILURE) {
			return;
		}
		XMLWRITER_FROM_OBJECT(intern, this);
	} else
#endif
	{
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsss!", &pind, 
			&prefix, &prefix_len, &name, &name_len, &uri, &uri_len) == FAILURE) {
			return;
		}
		ZEND_FETCH_RESOURCE(intern,xmlwriter_object *, &pind, -1, "XMLWriter", le_xmlwriter);
	}

	XMLW_NAME_CHK("Invalid Attribute Name");

	ptr = intern->ptr;

	if (ptr) {
		retval = xmlTextWriterStartAttributeNS(ptr, (xmlChar *)prefix, (xmlChar *)name, (xmlChar *)uri);
		if (retval != -1) {
			RETURN_TRUE;
		}
	}
	
	RETURN_FALSE;
}
