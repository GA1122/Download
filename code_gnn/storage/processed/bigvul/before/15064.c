int dom_document_document_uri_write(dom_object *obj, zval *newval TSRMLS_DC)
{
	zval value_copy;
	xmlDoc *docp;

	docp = (xmlDocPtr) dom_object_get_node(obj);

	if (docp == NULL) {
		php_dom_throw_error(INVALID_STATE_ERR, 0 TSRMLS_CC);
		return FAILURE;
	}

	if (docp->URL != NULL) {
		xmlFree((xmlChar *) docp->URL);
	}

	if (newval->type != IS_STRING) {
		if(Z_REFCOUNT_P(newval) > 1) {
			value_copy = *newval;
			zval_copy_ctor(&value_copy);
			newval = &value_copy;
		}
		convert_to_string(newval);
	}

	docp->URL = xmlStrdup((const xmlChar *) Z_STRVAL_P(newval));

	if (newval == &value_copy) {
		zval_dtor(newval);
	}

	return SUCCESS;
}
