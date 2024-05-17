PHP_FUNCTION(dom_document_create_processing_instruction)
{
	zval *id;
	xmlNode *node;
	xmlDocPtr docp;
	int ret, value_len, name_len = 0;
	dom_object *intern;
	char *name, *value = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|s", &id, dom_document_class_entry, &name, &name_len, &value, &value_len) == FAILURE) {
		return;
	}

	DOM_GET_OBJ(docp, id, xmlDocPtr, intern);

	if (xmlValidateName((xmlChar *) name, 0) != 0) {
		php_dom_throw_error(INVALID_CHARACTER_ERR, dom_get_strict_error(intern->document) TSRMLS_CC);
		RETURN_FALSE;
	}

	node = xmlNewPI((xmlChar *) name, (xmlChar *) value);
	if (!node) {
		RETURN_FALSE;
	}

	node->doc = docp;

	DOM_RET_OBJ(node, &ret, intern);
}
