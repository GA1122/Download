int dom_document_document_uri_read(dom_object *obj, zval **retval TSRMLS_DC)
{
	xmlDoc *docp;
	char *url;

	docp = (xmlDocPtr) dom_object_get_node(obj);

	if (docp == NULL) {
		php_dom_throw_error(INVALID_STATE_ERR, 0 TSRMLS_CC);
		return FAILURE;
	}

	ALLOC_ZVAL(*retval);
	url = (char *) docp->URL;
	if (url != NULL) {
		ZVAL_STRING(*retval, url, 1);
	} else {
		ZVAL_NULL(*retval);
	}

	return SUCCESS;
}
