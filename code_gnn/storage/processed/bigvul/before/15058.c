PHP_METHOD(domdocument, registerNodeClass)
{
	zval *id;
	xmlDoc *docp;
	char *baseclass = NULL, *extendedclass = NULL;
	int baseclass_len = 0, extendedclass_len = 0;
	zend_class_entry *basece = NULL, *ce = NULL;
	dom_object *intern;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss!", &id, dom_document_class_entry, &baseclass, &baseclass_len, &extendedclass, &extendedclass_len) == FAILURE) {
		return;
	}

	if (baseclass_len) {
		zend_class_entry **pce;
		if (zend_lookup_class(baseclass, baseclass_len, &pce TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class %s does not exist", baseclass);
			return;
		}
		basece = *pce;
	}

	if (basece == NULL || ! instanceof_function(basece, dom_node_class_entry TSRMLS_CC)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class %s is not derived from DOMNode.", baseclass);
		return;
	}

	if (extendedclass_len) {
		zend_class_entry **pce;
		if (zend_lookup_class(extendedclass, extendedclass_len, &pce TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class %s does not exist", extendedclass);
		}
		ce = *pce;
	}

	if (ce == NULL || instanceof_function(ce, basece TSRMLS_CC)) {

		DOM_GET_OBJ(docp, id, xmlDocPtr, intern);

		if (dom_set_doc_classmap(intern->document, basece, ce TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class %s could not be registered.", extendedclass);
		}
		RETURN_TRUE;
	} else {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class %s is not derived from %s.", extendedclass, baseclass);
	}

	RETURN_FALSE;
}
