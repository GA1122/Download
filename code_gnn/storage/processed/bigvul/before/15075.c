int dom_document_standalone_read(dom_object *obj, zval **retval TSRMLS_DC)
{
	xmlDoc *docp;
	int standalone;

	docp = (xmlDocPtr) dom_object_get_node(obj);

	if (docp == NULL) {
		php_dom_throw_error(INVALID_STATE_ERR, 0 TSRMLS_CC);
		return FAILURE;
	}

	ALLOC_ZVAL(*retval);
	standalone = docp->standalone;
	ZVAL_BOOL(*retval, standalone);

	return SUCCESS;
}
