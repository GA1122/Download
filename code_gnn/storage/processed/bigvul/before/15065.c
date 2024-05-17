int dom_document_encoding_read(dom_object *obj, zval **retval TSRMLS_DC)
{
	xmlDoc *docp;
	char *encoding;

	docp = (xmlDocPtr) dom_object_get_node(obj);

	if (docp == NULL) {
		php_dom_throw_error(INVALID_STATE_ERR, 0 TSRMLS_CC);
		return FAILURE;
	}

	encoding = (char *) docp->encoding;
	ALLOC_ZVAL(*retval);

	if (encoding != NULL) {
		ZVAL_STRING(*retval, encoding, 1);
	} else {
		ZVAL_NULL(*retval);
	}

	return SUCCESS;
}
