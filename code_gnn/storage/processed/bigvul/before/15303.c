static PHP_FUNCTION(xmlwriter_write_element)
{
	zval *pind;
	xmlwriter_object *intern;
	xmlTextWriterPtr ptr;
	char *name, *content = NULL;
	int name_len, content_len, retval;

#ifdef ZEND_ENGINE_2
	zval *this = getThis();
	
	if (this) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s!",
			&name, &name_len, &content, &content_len) == FAILURE) {
			return;
		}
		XMLWRITER_FROM_OBJECT(intern, this);
	} else
#endif
	{
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|s!", &pind, 
			&name, &name_len, &content, &content_len) == FAILURE) {
			return;
		}
		ZEND_FETCH_RESOURCE(intern,xmlwriter_object *, &pind, -1, "XMLWriter", le_xmlwriter);
	}

	XMLW_NAME_CHK("Invalid Element Name");

	ptr = intern->ptr;

	if (ptr) {
		if (!content) {
			retval = xmlTextWriterStartElement(ptr, (xmlChar *)name);
            if (retval == -1) {
                RETURN_FALSE;
            }
			xmlTextWriterEndElement(ptr);
            if (retval == -1) {
                RETURN_FALSE;
            }
		} else {
			retval = xmlTextWriterWriteElement(ptr, (xmlChar *)name, (xmlChar *)content);
		}
		if (retval != -1) {
			RETURN_TRUE;
		}
	}
	
	RETURN_FALSE;
}
