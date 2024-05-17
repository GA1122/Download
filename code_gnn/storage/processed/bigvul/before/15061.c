int dom_document_doctype_read(dom_object *obj, zval **retval TSRMLS_DC)
{
	xmlDoc *docp;
	xmlDtdPtr dtdptr;
	int ret;

	docp = (xmlDocPtr) dom_object_get_node(obj);

	if (docp == NULL) {
		php_dom_throw_error(INVALID_STATE_ERR, 0 TSRMLS_CC);
		return FAILURE;
	}

	ALLOC_ZVAL(*retval);

	dtdptr = xmlGetIntSubset(docp);
	if (!dtdptr) {
		ZVAL_NULL(*retval);
		return SUCCESS;
	}

	if (NULL == (*retval = php_dom_create_object((xmlNodePtr) dtdptr, &ret, *retval, obj TSRMLS_CC))) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot create required DOM object");
		return FAILURE;
	}
	return SUCCESS;
	
}
