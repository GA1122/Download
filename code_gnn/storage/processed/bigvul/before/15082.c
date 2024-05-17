int dom_document_version_read(dom_object *obj, zval **retval TSRMLS_DC)
{
	xmlDoc *docp;
	char *version;

	docp = (xmlDocPtr) dom_object_get_node(obj);

	if (docp == NULL) {
		php_dom_throw_error(INVALID_STATE_ERR, 0 TSRMLS_CC);
		return FAILURE;
	}

	version = (char *) docp->version;
	ALLOC_ZVAL(*retval);

	if (version != NULL) {
		ZVAL_STRING(*retval, version, 1);
	} else {
		ZVAL_NULL(*retval);
	}

	return SUCCESS;
}
