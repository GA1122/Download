int dom_document_standalone_write(dom_object *obj, zval *newval TSRMLS_DC)
{
	zval value_copy;
	xmlDoc *docp;
	int standalone;

	docp = (xmlDocPtr) dom_object_get_node(obj);

	if (docp == NULL) {
		php_dom_throw_error(INVALID_STATE_ERR, 0 TSRMLS_CC);
		return FAILURE;
	}

	if(Z_REFCOUNT_P(newval) > 1) {
		value_copy = *newval;
		zval_copy_ctor(&value_copy);
		newval = &value_copy;
	}
	convert_to_long(newval);

	standalone = Z_LVAL_P(newval);
    if (standalone > 0) {
        docp->standalone = 1;
    }
    else if (standalone < 0) {
        docp->standalone = -1;
    }
    else {
        docp->standalone = 0;
    }

	if (newval == &value_copy) {
		zval_dtor(newval);
	}

	return SUCCESS;
}
