static PHP_FUNCTION(xmlwriter_write_dtd_entity)
{
	zval *pind;
	xmlwriter_object *intern;
	xmlTextWriterPtr ptr;
	char *name, *content;
	int name_len, content_len, retval;
	 
	char *pubid = NULL, *sysid = NULL, *ndataid = NULL;
	zend_bool pe = 0;
	int pubid_len, sysid_len, ndataid_len;

#ifdef ZEND_ENGINE_2
	zval *this = getThis();

	if (this) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|bsss",
			&name, &name_len, &content, &content_len, &pe, &pubid, &pubid_len,
			&sysid, &sysid_len, &ndataid, &ndataid_len) == FAILURE) {
			return;
		}
		XMLWRITER_FROM_OBJECT(intern, this);
	} else
#endif
	{
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss|bsss", &pind, 
			&name, &name_len, &content, &content_len, &pe, &pubid, &pubid_len,
			&sysid, &sysid_len, &ndataid, &ndataid_len) == FAILURE) {
			return;
		}
		ZEND_FETCH_RESOURCE(intern,xmlwriter_object *, &pind, -1, "XMLWriter", le_xmlwriter);
	}

	XMLW_NAME_CHK("Invalid Element Name");

	ptr = intern->ptr;

	if (ptr) {
		retval = xmlTextWriterWriteDTDEntity(ptr, pe, (xmlChar *)name, (xmlChar *)pubid, (xmlChar *)sysid, (xmlChar *)ndataid, (xmlChar *)content);
		if (retval != -1) {
			RETURN_TRUE;
		}
	}
	
	RETURN_FALSE;
}