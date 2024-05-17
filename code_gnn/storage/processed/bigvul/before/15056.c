PHP_METHOD(domdocument, __construct)
{

	zval *id;
	xmlDoc *docp = NULL, *olddoc;
	dom_object *intern;
	char *encoding, *version = NULL;
	int encoding_len = 0, version_len = 0, refcount;
	zend_error_handling error_handling;

	zend_replace_error_handling(EH_THROW, dom_domexception_class_entry, &error_handling TSRMLS_CC);
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|ss", &id, dom_document_class_entry, &version, &version_len, &encoding, &encoding_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}

	zend_restore_error_handling(&error_handling TSRMLS_CC);
	docp = xmlNewDoc(version);

	if (!docp) {
		php_dom_throw_error(INVALID_STATE_ERR, 1 TSRMLS_CC);
		RETURN_FALSE;
	}

	if (encoding_len > 0) {
		docp->encoding = (const xmlChar*)xmlStrdup(encoding);
	}

	intern = (dom_object *)zend_object_store_get_object(id TSRMLS_CC);
	if (intern != NULL) {
		olddoc = (xmlDocPtr) dom_object_get_node(intern);
		if (olddoc != NULL) {
			php_libxml_decrement_node_ptr((php_libxml_node_object *) intern TSRMLS_CC);
			refcount = php_libxml_decrement_doc_ref((php_libxml_node_object *)intern TSRMLS_CC);
			if (refcount != 0) {
				olddoc->_private = NULL;
			}
		}
		intern->document = NULL;
		if (php_libxml_increment_doc_ref((php_libxml_node_object *)intern, docp TSRMLS_CC) == -1) {
			RETURN_FALSE;
		}
		php_libxml_increment_node_ptr((php_libxml_node_object *)intern, (xmlNodePtr)docp, (void *)intern TSRMLS_CC);
	}
}
